const ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const ROTORS = {
  I: {
    wiring: "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
    notch: "Q"
  },
  II: {
    wiring: "AJDKSIRUXBLHWTMCQGZNPYFVOE",
    notch: "E"
  },
  III: {
    wiring: "BDFHJLCPRTXVZNYEIWGAKMUSQO",
    notch: "V"
  },
  IV: {
    wiring: "ESOVPZJAYQUIRHXLNFTGKDCMWB",
    notch: "J"
  },
  VIII: {
    wiring: "FKQHTLXOCBJSPDZRAMEWNIUYGV",
    notch: "ZM"
  }
};

const REFLECTORS = {
  B: "YRUHQSLDPXNGOKMIEBFZCWVJAT"
};

const VARIANT = {
  number: 9,
  leftRotor: "VIII",
  middleRotor: "II",
  rightRotor: "IV",
  reflector: "B",
  stepPattern: {
    left: 1,
    middle: 0,
    right: 1
  }
};

const MESSAGE = {
  original: "Pavlovich Yan Andreevich",
  normalized: "PAVLOVICHYANANDREEVICH"
};

const DEFAULT_RING_SETTINGS = {
  left: "A",
  middle: "A",
  right: "A"
};

const TASK1_SETTINGS = [
  "AAA",
  "ABC",
  "DOG",
  "KEY",
  "LAB",
  "MNS",
  "QWE",
  "RTY",
  "SUN",
  "ZEN"
];

const TASK2_SETTINGS = ["AAA", "DOG", "KEY", "QWE", "ZEN"];

module.exports = {
  ALPHABET,
  DEFAULT_RING_SETTINGS,
  MESSAGE,
  REFLECTORS,
  ROTORS,
  TASK1_SETTINGS,
  TASK2_SETTINGS,
  VARIANT
};
