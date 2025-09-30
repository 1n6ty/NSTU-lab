import express from "express";
import bodyParser from "body-parser";
import fs from 'fs';

const app = express();
const PORT = 3000;

app.use(bodyParser.json());
app.use(express.static('./assets'));
app.use(bodyParser.urlencoded({ extended: true }));

app.set("view engine", "ejs");
app.set("views", "./templates");

if(!fs.existsSync("./tmp")) fs.mkdirSync("./tmp");

/**
 * Clears all temporary files by rewriting them with empty strings.
 * Files: alphabet, text, encoded, decoded.
 */
function clear(){
  for(let p of ["./tmp/alphabet", "./tmp/text", "./tmp/encoded", "./tmp/decoded"]){
    fs.writeFileSync(p, "");
  }
}
clear();

app.get("/", (req, res) => {
  let alphabet = String(fs.readFileSync("./tmp/alphabet"));
  alphabet = alphabet.split('\n', 2);
  res.render("index", {prob: alphabet[0], sym: alphabet[1]});
});

/**
 * Builds Huffman codes from given symbols and probabilities.
 *
 * @param {string[]} symbols - Array of symbols.
 * @param {number[]} probabilities - Corresponding probabilities for each symbol.
 * @returns {Object} codes, decodes, avg_len, redundancy, craft
 */
function huffman_encoding(symbols, probabilities) {
  class Node {
    constructor(prob, symbol = null, left = null, right = null) {
      this.prob = prob;
      this.symbol = symbol;
      this.left = left;
      this.right = right;
    }
  }

  let heap = symbols.map((sym, i) => new Node(probabilities[i], sym));

  while (heap.length > 1) {
    heap.sort((a, b) => a.prob - b.prob);

    let left = heap.shift();
    let right = heap.shift();

    let merged = new Node(left.prob + right.prob, null, left, right);
    heap.push(merged);
  }

  let root = heap[0],
      codes = {},
      decodes = {},
      avg_len = 0,
      craft = 0;

  /**
   * Recursively traverses Huffman tree to generate codes.
   * @param {Node} node - Current tree node.
   * @param {string} code - Accumulated code string.
   */
  (function generateCodes(node, code = "") {
    if (node.symbol !== null) {
      code = code || "0";

      codes[node.symbol] = code;
      decodes[code] = node.symbol;
      avg_len += (code).length / symbols.length;
      craft += 2 ** (-(code).length);
      return;
    }
    generateCodes(node.left, code + "0");
    generateCodes(node.right, code + "1");
  })(root);

  let r = avg_len - probabilities.reduce((prev, e) => {return prev + e * Math.log2(1 / e);}, 0);

  return {codes: codes, decodes: decodes, avg_len: avg_len, r: r, craft: craft};
}

/**
 * Reads alphabet file and returns symbols with probabilities.
 *
 * @returns {Object|null} {sym: string[], prob: number[]} or null if no alphabet
 */
function get_alphabet(){
  let data = String(fs.readFileSync("./tmp/alphabet"));

  if(!data) return null;

  let prob_sym = data.split('\n', 2);
  return {
    sym: prob_sym[1].split(';'),
    prob: prob_sym[0].split(';').map(
      (e) => {
        return parseFloat(e);
      }
    )
  }
}

app.get("/decode/", (req, res) => {
  let encoded = String(fs.readFileSync("./tmp/encoded"));
  let decoded = String(fs.readFileSync("./tmp/decoded"));
  encoded += `\nlength = ${encoded.length}`;
  decoded += `\nlength = ${decoded.length}`;

  let sym_prob = get_alphabet();
  if(!sym_prob){
    res.render("decode", {text: encoded, decoded: "", props: "No alphabet was presented!"});
    return;
  }

  let {sym, prob} = sym_prob;
  let props = "", hcodes = huffman_encoding(sym, prob);
  for(let k in hcodes.codes) props += `${k} -> ${hcodes.codes[k]}\n`;
  props += `Average length: ${hcodes.avg_len}\n`;
  props += `Redundancy: ${hcodes.r}\n`;
  props += `Craft's inequality: ${hcodes.craft} <= 1`;

  res.render("decode", {text: encoded, decoded: decoded, props: props});
});
app.post("/decode/", (req, res) => {
  let text = String(fs.readFileSync("./tmp/encoded"));
  
  let sym_prob = get_alphabet();
  if(!sym_prob){
    res.json({ decoded: "" });
    return;
  }
  let {sym, prob} = sym_prob,
      hcodes = huffman_encoding(sym, prob);

  let decoded = "", buf = "";
  for(let s of text){
    buf += s;
    if(buf in hcodes.decodes){
      decoded += hcodes.decodes[buf];
      buf = "";
    }
  }
  fs.writeFileSync("./tmp/decoded", decoded);

  res.json({decoded: decoded + `\nlength = ${decoded.length}`});
});

app.get("/encode/", (req, res) => {
  let text = String(fs.readFileSync("./tmp/text"));
  let encoded = String(fs.readFileSync("./tmp/encoded"));
  text += `\nlength = ${text.length}`;
  encoded += `\nlength = ${encoded.length}`;

  let sym_prob = get_alphabet();
  if(!sym_prob){
    res.render("encode", {text: text, encoded: "", props: "No alphabet was presented!"});
    return;
  }

  let {sym, prob} = sym_prob;
  let props = "", hcodes = huffman_encoding(sym, prob);
  for(let k in hcodes.codes) props += `${k} -> ${hcodes.codes[k]}\n`;
  props += `Average length: ${hcodes.avg_len}\n`;
  props += `Redundancy: ${hcodes.r}\n`;
  props += `Craft's inequality: ${hcodes.craft} <= 1`;

  res.render("encode", {text: text, encoded: encoded, props: props});
});
app.post("/encode/", (req, res) => {
  let text = String(fs.readFileSync("./tmp/text"));
  
  let sym_prob = get_alphabet();
  if(!sym_prob){
    res.json({ encoded: "" });
    return;
  }
  let {sym, prob} = sym_prob,
      hcodes = huffman_encoding(sym, prob);

  let encoded = "", buf = "";
  for(let s of text){
    buf += s;
    if(buf in hcodes.codes){
      encoded += hcodes.codes[buf];
      buf = "";
    }
  }
  fs.writeFileSync("./tmp/encoded", encoded);

  res.json({encoded: encoded + `\nlength = ${encoded.length}`});
});

app.post("/prob/", (req, res) => {
  let {prob, sym} = req.body;

  prob = prob.split(';').filter(Boolean);
  sym = sym.split(';').filter(Boolean);

  if(prob.length != sym.length){
    fs.writeFileSync("./tmp/alphabet", "");
    res.json({ err: "Count of symbols and probabilities didn't match!" });
    return;
  }
  
  if(Math.abs(prob.reduce((prev, e) => {return prev + parseFloat(e);}, 0) - 1) > 0.1){
    fs.writeFileSync("./tmp/alphabet", "");
    res.json({ err: "Sum of probabilities should be equal to one!" });
    return;
  }

  fs.writeFile("./tmp/alphabet", `${prob.join(';')}\n${sym.join(';')}`, function(err) {
      if(err) {
          console.log(err);
          return;
      }
  });

  res.json({ err: "" });
});

/**
 * Generates a random symbol based on given probabilities.
 *
 * @param {string[]} sym - Array of symbols.
 * @param {number[]} prob - Array of probabilities for each symbol.
 * @returns {string} Randomly chosen symbol.
 */
function generate_p_symbol(sym, prob){
  let rnum = Math.random(),
      p = 0;
  for(let i = 0; i < prob.length; i++){
    p += prob[i];
    if(rnum <= p) return sym[i];
  }
  return sym[sym.length - 1];
}

app.get("/gentext/", (req, res) => {
  let text = String(fs.readFileSync("./tmp/text"));
  res.render("textgen", {text: text, len: text.length});
});
app.post("/gentext/", (req, res) => {
  for(let i of ["encoded", "decoded"]) fs.writeFileSync(`./tmp/${i}`, "");
  let {len} = req.body,
      text = "";

  let sym_prob = get_alphabet();
  if(!sym_prob){
    res.json({ text: "" });
    return;
  }
  let {sym, prob} = sym_prob;
    
  for(let i = 0; i < parseInt(len); i++){
    text += generate_p_symbol(sym, prob);
  }

  fs.writeFile("./tmp/text", text, function(err) {
      if(err) {
          return console.log(err);
      }
      res.json({ text: text });
  });
});

app.post('/clear/', (req, res) => {
  clear();
  res.json({status: 'done'});
})

// Start server
app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});
