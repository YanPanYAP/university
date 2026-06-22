const { ALPHABET, REFLECTORS, ROTORS, VARIANT } = require("./config");

function letterToIndex(letter) {
  return ALPHABET.indexOf(letter);
}

function indexToLetter(index) {
  return ALPHABET[((index % 26) + 26) % 26];
}

function rotorAt(name, positionLetter, ringLetter) {
  const rotor = ROTORS[name];

  if (!rotor) {
    throw new Error(`Неизвестный ротор: ${name}`);
  }

  return {
    name,
    wiring: rotor.wiring,
    inverseWiring: invertWiring(rotor.wiring),
    notch: rotor.notch,
    position: letterToIndex(positionLetter),
    ring: letterToIndex(ringLetter)
  };
}

function invertWiring(wiring) {
  const inverse = new Array(26);

  for (let index = 0; index < wiring.length; index++) {
    inverse[letterToIndex(wiring[index])] = indexToLetter(index);
  }

  return inverse.join("");
}

function stepRotor(rotor, stepCount) {
  const previous = rotor.position;
  rotor.position = (rotor.position + stepCount) % 26;
  return previous + stepCount >= 26;
}

function encodeForward(index, rotor) {
  const shifted = (index + rotor.position - rotor.ring + 26) % 26;
  const wired = letterToIndex(rotor.wiring[shifted]);
  return (wired - rotor.position + rotor.ring + 26) % 26;
}

function encodeBackward(index, rotor) {
  const shifted = (index + rotor.position - rotor.ring + 26) % 26;
  const wired = letterToIndex(rotor.inverseWiring[shifted]);
  return (wired - rotor.position + rotor.ring + 26) % 26;
}

function stepRotors(machine) {
  const { stepPattern } = machine;
  const rightCompletedTurn = stepPattern.right > 0
    ? stepRotor(machine.right, stepPattern.right)
    : false;

  if (stepPattern.middle > 0) {
    stepRotor(machine.middle, stepPattern.middle);
  } else if (rightCompletedTurn) {
    stepRotor(machine.middle, 1);
  }

  if (stepPattern.left > 0) {
    stepRotor(machine.left, stepPattern.left);
  }
}

function createMachine(options = {}) {
  const rotorOrder = options.rotorOrder || {
    left: VARIANT.leftRotor,
    middle: VARIANT.middleRotor,
    right: VARIANT.rightRotor
  };
  const ringSettings = options.ringSettings || { left: "A", middle: "A", right: "A" };
  const initialPositions = options.initialPositions || { left: "A", middle: "A", right: "A" };
  const reflectorName = options.reflector || VARIANT.reflector;

  return {
    stepPattern: options.stepPattern || {
      left: VARIANT.stepPattern.left,
      middle: VARIANT.stepPattern.middle,
      right: VARIANT.stepPattern.right
    },
    left: rotorAt(rotorOrder.left, initialPositions.left, ringSettings.left),
    middle: rotorAt(rotorOrder.middle, initialPositions.middle, ringSettings.middle),
    right: rotorAt(rotorOrder.right, initialPositions.right, ringSettings.right),
    reflector: REFLECTORS[reflectorName]
  };
}

function parsePositionString(positionString) {
  const normalized = positionString.toUpperCase();

  if (!/^[A-Z]{3}$/.test(normalized)) {
    throw new Error("Начальная установка роторов должна содержать три буквы A-Z.");
  }

  return {
    left: normalized[0],
    middle: normalized[1],
    right: normalized[2]
  };
}

function formatPosition(machine) {
  return [machine.left, machine.middle, machine.right]
    .map((rotor) => indexToLetter(rotor.position))
    .join("");
}

function traceForward(inputLetter, rotor) {
  const inputIndex = letterToIndex(inputLetter);
  const shiftedIndex = (inputIndex + rotor.position - rotor.ring + 26) % 26;
  const shiftedLetter = indexToLetter(shiftedIndex);
  const wiredLetter = rotor.wiring[shiftedIndex];
  const outputIndex = (letterToIndex(wiredLetter) - rotor.position + rotor.ring + 26) % 26;
  const outputLetter = indexToLetter(outputIndex);

  return {
    rotor: rotor.name,
    input: inputLetter,
    shifted: shiftedLetter,
    wired: wiredLetter,
    output: outputLetter
  };
}

function traceBackward(inputLetter, rotor) {
  const inputIndex = letterToIndex(inputLetter);
  const shiftedIndex = (inputIndex + rotor.position - rotor.ring + 26) % 26;
  const shiftedLetter = indexToLetter(shiftedIndex);
  const wiredLetter = rotor.inverseWiring[shiftedIndex];
  const outputIndex = (letterToIndex(wiredLetter) - rotor.position + rotor.ring + 26) % 26;
  const outputLetter = indexToLetter(outputIndex);

  return {
    rotor: rotor.name,
    input: inputLetter,
    shifted: shiftedLetter,
    wired: wiredLetter,
    output: outputLetter
  };
}

function encrypt(text, options = {}) {
  const machine = createMachine({
    ...options,
    initialPositions: parsePositionString(options.initialPosition || "AAA")
  });
  let output = "";
  const traces = [];

  for (const [index, char] of Array.from(text).entries()) {
    if (!ALPHABET.includes(char)) {
      continue;
    }

    stepRotors(machine);
    const position = formatPosition(machine);
    const forwardRight = traceForward(char, machine.right);
    const forwardMiddle = traceForward(forwardRight.output, machine.middle);
    const forwardLeft = traceForward(forwardMiddle.output, machine.left);
    const reflectorInput = forwardLeft.output;
    const reflectorOutput = machine.reflector[letterToIndex(reflectorInput)];
    const backwardLeft = traceBackward(reflectorOutput, machine.left);
    const backwardMiddle = traceBackward(backwardLeft.output, machine.middle);
    const backwardRight = traceBackward(backwardMiddle.output, machine.right);
    const outputLetter = backwardRight.output;

    output += outputLetter;
    traces.push({
      step: index + 1,
      position,
      input: char,
      output: outputLetter,
      path: {
        forward: [forwardRight, forwardMiddle, forwardLeft],
        reflector: {
          input: reflectorInput,
          output: reflectorOutput
        },
        backward: [backwardLeft, backwardMiddle, backwardRight]
      }
    });
  }

  return {
    ciphertext: output,
    finalPosition: formatPosition(machine),
    traces
  };
}

function decrypt(text, options = {}) {
  const result = encrypt(text, options);

  return {
    plaintext: result.ciphertext,
    finalPosition: result.finalPosition,
    traces: result.traces
  };
}

module.exports = {
  createMachine,
  decrypt,
  encrypt,
  formatPosition,
  parsePositionString
};
