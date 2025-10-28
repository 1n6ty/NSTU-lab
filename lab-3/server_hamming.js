import express from "express";
import bodyParser from "body-parser";
import fs from "fs";

const app = express();
const PORT = 3001;

// ===================================================================
//  Server & Middleware Configuration
// ===================================================================

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static("./assets"));

app.set("view engine", "ejs");
app.set("views", "./templates");

if (!fs.existsSync("./tmp")) fs.mkdirSync("./tmp");

/**
 * Clears all temporary files used for text, encoded, and decoded data.
 */
const clear = () => {
  ["./tmp/text_ham", "./tmp/encoded_ham", "./tmp/decoded_ham"].forEach((p) =>
    fs.writeFileSync(p, "", "utf8")
  );
};
clear();

// ===================================================================
//  Alphabet & Bitwise Utilities
// ===================================================================

/**
 * Returns the alphabet of 32 symbols (0–31) with uniform probabilities.
 * @returns {{sym: number[], prob: number[]}} Symbols and their probabilities.
 */
const getAlphabet = () => ({
  sym: Array.from({ length: 32 }, (_, i) => i),
  prob: Array(32).fill(1 / 32),
});

/**
 * Converts an integer [0–31] into a 5-bit binary array.
 * @param {number} num - Integer to convert.
 * @returns {number[]} 5-bit binary representation.
 * @throws {Error} If num is outside [0, 31].
 */
const to5BitArray = (num) => {
  if (num < 0 || num > 31) throw new Error("Number must be in [0, 31]");
  return Array.from({ length: 5 }, (_, i) => (num >> (4 - i)) & 1);
};

/**
 * Multiplies a vector by a binary matrix modulo 2.
 * @param {number[]} vec - Input binary vector.
 * @param {number[][]} M - Binary matrix.
 * @returns {number[]} Result of multiplication modulo 2.
 */
const mul = (vec, M) =>
  M[0].map((_, col) =>
    M.reduce((sum, row, rowIndex) => (sum + row[col] * vec[rowIndex]) % 2, 0)
  );

// ===================================================================
//  Hamming Code Implementation
// ===================================================================

/**
 * Encodes a 5-bit vector using the (9,5) Hamming generator matrix.
 * @param {number[]} vec - Input 5-bit vector.
 * @returns {number[]} Encoded 9-bit Hamming code.
 */
const encodeHamming = (vec) => {
  const G = [
    [1, 0, 0, 0, 0, 0, 0, 1, 1],
    [0, 1, 0, 0, 0, 0, 1, 0, 1],
    [0, 0, 1, 0, 0, 0, 1, 1, 0],
    [0, 0, 0, 1, 0, 0, 1, 1, 1],
    [0, 0, 0, 0, 1, 1, 0, 0, 1],
  ];
  return mul(vec, G);
};

/**
 * Computes the syndrome of a received 9-bit vector.
 * Used to detect the position of single-bit errors.
 * @param {number[]} vec - Received 9-bit vector.
 * @returns {number} Syndrome as an integer.
 */
const computeSyndrome = (vec) => {
  const HT = [
    [0, 0, 1, 1],
    [0, 1, 0, 1],
    [0, 1, 1, 0],
    [0, 1, 1, 1],
    [1, 0, 0, 1],
    [1, 0, 0, 0],
    [0, 1, 0, 0],
    [0, 0, 1, 0],
    [0, 0, 0, 1],
  ];

  const r = mul(vec, HT);
  return r.reduce((sum, bit, i) => sum + (bit ? 2 ** (r.length - 1 - i) : 0), 0);
};

/**
 * Maps a syndrome value to a bit position for correction.
 * @param {number} syndrome - Syndrome integer.
 * @returns {number} Bit index to flip or -1 if not found.
 */
const getSyndromePosition = (syndrome) =>
  [3, 5, 6, 7, 9, 1, 2, 4, 8].indexOf(syndrome);

/**
 * Attempts to decode a 9-bit binary string into its original 5-bit symbol.
 * @param {string} str - 9-bit binary string.
 * @returns {number|string} Decoded symbol or '?' if unrecognized.
 */
const decodeSymbol = (str) => {
  const { sym } = getAlphabet();
  const prefix = str.slice(0, 5);
  const match = sym.find((i) => to5BitArray(i).join("") === prefix);
  return match ?? "?";
};

// ===================================================================
//  Utility
// ===================================================================


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
  const k = n - 4;
  const hammingEdge = `${4} >= ${Math.log2([0, 1].reduce((acc, ch) => acc + C(n, ch), 0)).toFixed(3)}`;
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
 * Generates a random symbol based on given probabilities.
 * @param {string[]} sym - Array of symbols.
 * @param {number[]} prob - Probability distribution.
 * @returns {string} Randomly selected symbol.
 */
const generateSymbol = (sym, prob) => {
  const r = Math.random();
  let acc = 0;
  for (let i = 0; i < prob.length; i++) {
    acc += prob[i];
    if (r <= acc) return sym[i];
  }
  return sym.at(-1);
};

// ===================================================================
//  Routes: Text Generation
// ===================================================================

app.get(["/", "/gentext"], (req, res) => {
  const text = fs.readFileSync("./tmp/text_ham", "utf8");
  res.render("textgen", { text, len: text.length });
});

app.post("/gentext/", (req, res) => {
  ["encoded", "decoded"].forEach((f) => fs.writeFileSync(`./tmp/${f}_ham`, ""));
  const { len } = req.body;
  const { sym, prob } = getAlphabet();

  const text = Array.from({ length: parseInt(len) }, () =>
    generateSymbol(sym, prob)
  ).join(" ");

  fs.writeFile("./tmp/text_ham", text, (err) => {
    if (err) return res.status(500).json({ error: err.message });
    res.json({ text });
  });
});

// ===================================================================
//  Routes: Encoding
// ===================================================================

app.get("/encode/", (req, res) => {
  const text = fs.readFileSync("./tmp/text_ham", "utf8");
  const encoded = fs.readFileSync("./tmp/encoded_ham", "utf8");

  const alphabet = getAlphabet();
  const codes = {};

  alphabet.sym.forEach((s) => {
    const hammingCode = encodeHamming(to5BitArray(s)).join("");
    codes[s] = hammingCode;
  });

  let props = alphabet.sym
    .map(
      (i) =>
        `${i} -> ${to5BitArray(i).join("")} -> ${encodeHamming(
          to5BitArray(i)
        ).join("")}`
    )
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
  const text = fs.readFileSync("./tmp/text_ham", "utf8");
  const encoded = text
    .split(" ")
    .map((n) => encodeHamming(to5BitArray(parseInt(n))).join(""))
    .join("");

  fs.writeFileSync("./tmp/encoded_ham", encoded);
  res.json({ encoded });
});

// ===================================================================
//  Routes: Decoding
// ===================================================================

app.get("/decode/", (req, res) => {
  const encoded = fs.readFileSync("./tmp/encoded_ham", "utf8");
  const decoded = fs.readFileSync("./tmp/decoded_ham", "utf8");

  const alphabet = getAlphabet();
  const codes = {};

  alphabet.sym.forEach((s) => {
    const hammingCode = encodeHamming(to5BitArray(s)).join("");
    codes[s] = hammingCode;
  });

  let props = alphabet.sym
    .map(
      (i) =>
        `${i} -> ${to5BitArray(i).join("")} -> ${encodeHamming(
          to5BitArray(i)
        ).join("")}`
    )
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
  const text = fs.readFileSync("./tmp/encoded_ham", "utf8");
  const chunks = text.match(/.{1,9}/g) || [];
  let msg = "";

  const decoded = chunks.map((chunk, idx) => {
    const bits = chunk.split("").map(Number);
    const s = computeSyndrome(bits);
    if (s) {
      const pos = getSyndromePosition(s);
      if (pos >= 0) {
        bits[pos] = (bits[pos] + 1) % 2;
        msg += `Fixed bit error at symbol ${idx}, bit position ${pos}.\n`;
      }
    }
    const decodedVal = decodeSymbol(bits.join(""));
    if (decodedVal === "?") msg += `Unrecognized symbol at position ${idx}.\n`;
    return decodedVal;
  });

  const decodedStr = decoded.join(" ");
  fs.writeFileSync("./tmp/decoded_ham", decodedStr);

  res.json({ decoded: `${decodedStr}\n\n${msg}` });
});

// ===================================================================
//  Routes: Utility
// ===================================================================

app.post("/clear/", (req, res) => {
  clear();
  res.json({ status: "done" });
});

// ===================================================================
//  Server Startup
// ===================================================================

app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}\n`);
});
