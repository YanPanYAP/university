const fs = require("fs");

// Павлович Ян Андрійович
// Pavlovich Jan Andreevich
const fio = "Pavlovich Jan Andreevich"; 
const buffer = Buffer.from(fio, "utf8");

fs.writeFileSync("dutch.bin", buffer);