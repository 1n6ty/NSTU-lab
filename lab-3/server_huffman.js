import express from "express";
import bodyParser from "body-parser";
import fs from "fs";

const app = express();
const PORT = 3000;

// =============================
// Middleware & Server Setup
// =============================
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static("./assets"));

app.set("view engine", "ejs");
app.set("views", "./templates");

// Ensure tmp directory exists
if (!fs.existsSync("./tmp")) fs.mkdirSync("./tmp");

/**
 * Clears temporary files for alphabet, text, encoded, decoded data.
 */
const clear = () => {
  ["./tmp/alphabet", "./tmp/text", "./tmp/encoded", "./tmp/decoded"].forEach((p) =>
    fs.writeFileSync(p, "", "utf8")
  );
};
clear();

// =============================
// Utility Functions
// =============================

/**
 * Computes Hamming distance between two equal-length binary strings.
 * @param {string} a
 * @param {string} b
 * @returns {number} Distance
 */
const hammingDistance = (a, b) =>
  a.split("").reduce((acc, ch, i) => acc + (ch !== b[i] ? 1 : 0), 0);

/**
 * Computes C(n, i)
 * @param {number} n
 * @param {number} i
 * @returns {number} C(n, i)
 */
const C = (n, i) => {
  if (i < 0 || i > n) return 0;
  if (i > n - i) i = n - i; // symmetry
  let res = 1;
  for (let k = 1; k <= i; k++) {
    res *= (n - k + 1) / k;
  }
  return res;
};

/**
 * Computes all pairwise Hamming distances for codewords.
 * @param {Object} codes - Symbol-to-code map
 * @returns {{pairs: string[], distances: number[], minDist: number}}
 */
const computeHammingMetrics = (codes) => {
  const keys = Object.keys(codes);
  const n = keys.length;
  const pairs = [];
  const distances = [];
  for (let i = 0; i < n; i++) {
    for (let j = i + 1; j < n; j++) {
      const d = hammingDistance(codes[keys[i]], codes[keys[j]]);
      pairs.push(`${keys[i]}-${keys[j]}`);
      distances.push(d);
    }
  }
  const minDist = Math.min(...distances);
  return { pairs, distances, minDist };
};

/**
 * Computes Hamming, Plotkin, and Varshamov bounds.
 * @param {number} M - Number of codewords
 * @param {number} n - Codeword length
 * @param {number} dmin - minimum hamming distance
 * @returns {Object}
 */
const computeBounds = (M, n, dmin) => {
  const k = n - 1;
  const hammingEdge = `${1} >= ${Math.log2([0].reduce((acc, ch) => acc + C(n, ch), 0)).toFixed(3)}`;
  const plotkinBound = `${dmin} <= ${(n * (2 ** (k - 1)) / (2 ** (k - 1) - 1)).toFixed(3)}`;

  let varr = [];
  for(let i = 0; i <= dmin - 2; i ++) varr.push(i);
  const varshamovBound = `${2 ** (n - k)} > ${varr.reduce((acc, ch) => acc + C(n - 1, ch), 0).toFixed(3)}`;

  return {
    hammingEdge: hammingEdge,
    plotkinBound: plotkinBound,
    varshamovBound: varshamovBound,
  };
};

/**
 * Reads the alphabet file and returns symbols with their probabilities.
 * @returns {{sym: string[], prob: number[]} | null} Symbols and probabilities or null if file is empty
 */
const getAlphabet = () => {
  const data = fs.readFileSync("./tmp/alphabet", "utf8");
  if (!data) return null;
  const [probLine, symLine] = data.split("\n", 2);
  return {
    sym: symLine.split(";"),
    prob: probLine.split(";").map(Number),
  };
};

/**
 * Computes parity bit for a binary string.
 * @param {string} str - Binary string
 * @returns {'0'|'1'} Parity bit
 */
const getParityBit = (str) =>
  [...str].reduce((prev, e) => (prev === e ? "0" : "1"), "0");

/**
 * Generates a random symbol based on provided probabilities.
 * @param {string[]} sym - Symbols
 * @param {number[]} prob - Probabilities
 * @returns {string} Randomly selected symbol
 */
const generateSymbol = (sym, prob) => {
  const r = Math.random();
  let acc = 0;
  for (let i = 0; i < sym.length; i++) {
    acc += prob[i];
    if (r <= acc) return sym[i];
  }
  return sym.at(-1);
};

/**
 * Huffman encoding with code generation and decoding table.
 * @param {string[]} symbols - Array of symbols
 * @param {number[]} probabilities - Corresponding probabilities
 * @returns {{codes: Object, decodes: Object}} Huffman codes and decoding table
 */
const huffmanEncoding = (symbols, probabilities) => {
  class Node {
    constructor(prob, symbol = null, left = null, right = null) {
      this.prob = prob;
      this.symbol = symbol;
      this.left = left;
      this.right = right;
    }
  }

  const heap = symbols.map((sym, i) => new Node(probabilities[i], sym));

  while (heap.length > 1) {
    heap.sort((a, b) => a.prob - b.prob);
    const left = heap.shift();
    const right = heap.shift();
    heap.push(new Node(left.prob + right.prob, null, left, right));
  }

  const codes = {};
  const decodes = {};

  /**
   * Recursive traversal to generate Huffman codes.
   * @param {Node} node 
   * @param {string} code 
   */
  const generateCodes = (node, code = "") => {
    if (node.symbol !== null) {
      code = code || "0"; // single-symbol case
      codes[node.symbol] = code;
      decodes[code] = node.symbol;
      return;
    }
    generateCodes(node.left, code + "0");
    generateCodes(node.right, code + "1");
  };

  generateCodes(heap[0]);

  // Pad codes to same length
  const maxLen = Math.max(...Object.values(codes).map((c) => c.length));
  Object.keys(codes).forEach((k) => (codes[k] = codes[k].padEnd(maxLen, "0")));
  Object.keys(decodes).forEach((k) => {
    const val = decodes[k];
    delete decodes[k];
    decodes[k.padEnd(maxLen, "0")] = val;
  });

  return { codes, decodes };
};

// =============================
// Routes
// =============================

app.get("/", (req, res) => {
  const alphabet = fs.readFileSync("./tmp/alphabet", "utf8").split("\n", 2);
  res.render("index", { prob: alphabet[0], sym: alphabet[1] });
});

// -----------------------------
// Encoding / Decoding Routes
// -----------------------------

app.get("/encode/", (req, res) => {
  const text = fs.readFileSync("./tmp/text", "utf8");
  const encoded = fs.readFileSync("./tmp/encoded", "utf8");

  const symProb = getAlphabet();
  if (!symProb) return res.render("encode", { text, encoded: "", props: "No alphabet was presented!" });

  const { sym, prob } = symProb;
  const { codes } = huffmanEncoding(sym, prob);
  let props = Object.entries(codes)
    .map(([k, c]) => `${k} -> ${c} + ${getParityBit(c)}`)
    .join("\n");

  const { pairs, distances, minDist } = computeHammingMetrics(codes);
  const bounds = computeBounds(Object.keys(codes).length, Object.values(codes)[0].length, minDist);

  props +=
    `\n\nCode metrics:\n` +
    `Min Hamming distance: ${minDist}\n` +
    `Hamming edge: ${bounds.hammingEdge}\n` +
    `Plotkin bound: ${bounds.plotkinBound}\n` +
    `Varshamov bound: ${bounds.varshamovBound}\n\nHamming distances:\n` +
    pairs.map((p, i) => `${p}: ${distances[i]}`).join("\n");

  res.render("encode", { text, encoded, props });
});

app.post("/encode/", (req, res) => {
  const text = fs.readFileSync("./tmp/text", "utf8");
  const symProb = getAlphabet();
  if (!symProb) return res.json({ encoded: "" });

  const { codes } = huffmanEncoding(symProb.sym, symProb.prob);
  let encoded = "";
  let buf = "";

  for (const s of text) {
    buf += s;
    if (buf in codes) {
      encoded += codes[buf] + getParityBit(codes[buf]);
      buf = "";
    }
  }

  fs.writeFileSync("./tmp/encoded", encoded);
  res.json({ encoded });
});

app.get("/decode/", (req, res) => {
  const encoded = fs.readFileSync("./tmp/encoded", "utf8");
  const decoded = fs.readFileSync("./tmp/decoded", "utf8");

  const symProb = getAlphabet();
  if (!symProb) return res.render("decode", { text: encoded, decoded: "", props: "No alphabet was presented!" });

  const { sym, prob } = symProb;
  const { codes } = huffmanEncoding(sym, prob);
  
  let props = Object.entries(codes)
    .map(([k, c]) => `${k} -> ${c} + ${getParityBit(c)}`)
    .join("\n");

  const { pairs, distances, minDist } = computeHammingMetrics(codes);
  const bounds = computeBounds(Object.keys(codes).length, Object.values(codes)[0].length, minDist);

  props +=
    `\n\nCode metrics:\n` +
    `Min Hamming distance: ${minDist}\n` +
    `Hamming edge: ${bounds.hammingEdge}\n` +
    `Plotkin bound: ${bounds.plotkinBound}\n` +
    `Varshamov bound: ${bounds.varshamovBound}\n\nHamming distances:\n` +
    pairs.map((p, i) => `${p}: ${distances[i]}`).join("\n");

  res.render("decode", { text: encoded, decoded, props });
});

app.post("/decode/", (req, res) => {
  const text = fs.readFileSync("./tmp/encoded", "utf8");
  const symProb = getAlphabet();
  if (!symProb) return res.json({ decoded: "" });

  const { codes, decodes } = huffmanEncoding(symProb.sym, symProb.prob);
  const codeLen = Object.values(codes)[0].length;

  let decoded = "";
  let msg = "";

  for (let i = 0; i < text.length - codeLen; i += codeLen + 1) {
    const buf = text.slice(i, i + codeLen);
    const parity = getParityBit(buf);
    const expected = text[i + codeLen];

    if (parity === expected) {
      decoded += decodes[buf] ?? "?";
      if (!decodes[buf]) msg += `Symbol does not exist at position ${i / (codeLen + 1)}, got ${buf}\n`;
    } else {
      decoded += "?";
      msg += `Parity mismatch at position ${i / (codeLen + 1)}: computed ${parity}, got ${expected}\n`;
    }
  }

  fs.writeFileSync("./tmp/decoded", decoded);
  res.json({ decoded: `${decoded}\n\n${msg}` });
});

// -----------------------------
// Probability and Text Generation
// -----------------------------

app.post("/prob/", (req, res) => {
  const sym = req.body.sym.split(";").filter(Boolean);
  const prob = Array(sym.length).fill(1 / sym.length);
  fs.writeFileSync("./tmp/alphabet", `${prob.join(";")}\n${sym.join(";")}`);
  res.json({ err: "" });
});

app.get("/gentext/", (req, res) => {
  const text = fs.readFileSync("./tmp/text", "utf8");
  res.render("textgen", { text, len: text.length });
});

app.post("/gentext/", (req, res) => {
  ["encoded", "decoded"].forEach((f) => fs.writeFileSync(`./tmp/${f}`, ""));
  const { len } = req.body;

  const symProb = getAlphabet();
  if (!symProb) return res.json({ text: "" });

  const { sym, prob } = symProb;
  const text = Array.from({ length: parseInt(len) }, () => generateSymbol(sym, prob)).join("");

  fs.writeFileSync("./tmp/text", text);
  res.json({ text });
});

// -----------------------------
// Clear endpoint
// -----------------------------
app.post("/clear/", (req, res) => {
  clear();
  res.json({ status: "done" });
});

// =============================
// Server Start
// =============================
app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});
