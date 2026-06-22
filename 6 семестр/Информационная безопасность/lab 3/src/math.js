function normalizeNumbers(values) {
  return values.map((value) => Number(value)).filter((value) => Number.isFinite(value));
}

function gcdTwo(a, b) {
  let left = Math.abs(a);
  let right = Math.abs(b);

  while (right !== 0) {
    const remainder = left % right;
    left = right;
    right = remainder;
  }

  return left;
}

function gcdMany(numbers) {
  if (numbers.length < 2 || numbers.length > 3) {
    throw new Error("НОД должен вычисляться для двух или трех чисел.");
  }

  return numbers.reduce((accumulator, current) => gcdTwo(accumulator, current));
}

function isPrime(value) {
  if (!Number.isInteger(value) || value < 2) {
    return false;
  }

  if (value === 2) {
    return true;
  }

  if (value % 2 === 0) {
    return false;
  }

  const limit = Math.floor(Math.sqrt(value));
  for (let divisor = 3; divisor <= limit; divisor += 2) {
    if (value % divisor === 0) {
      return false;
    }
  }

  return true;
}

function findPrimesInRange(from, to) {
  const start = Math.min(from, to);
  const end = Math.max(from, to);
  const primes = [];

  for (let value = start; value <= end; value++) {
    if (isPrime(value)) {
      primes.push(value);
    }
  }

  return primes;
}

function sievePrimes(limit) {
  if (limit < 2) {
    return {
      primes: [],
      crossedByBasePrime: {}
    };
  }

  const marked = Array(limit + 1).fill(true);
  marked[0] = false;
  marked[1] = false;
  const crossedByBasePrime = {};

  for (let base = 2; base * base <= limit; base++) {
    if (!marked[base]) {
      continue;
    }

    crossedByBasePrime[base] = [];
    for (let composite = base * base; composite <= limit; composite += base) {
      if (marked[composite]) {
        marked[composite] = false;
        crossedByBasePrime[base].push(composite);
      }
    }
  }

  const primes = [];
  for (let value = 2; value <= limit; value++) {
    if (marked[value]) {
      primes.push(value);
    }
  }

  return { primes, crossedByBasePrime };
}

function findPrimesWithSieve(from, to) {
  const start = Math.min(from, to);
  const end = Math.max(from, to);
  const sieve = sievePrimes(end);
  return {
    primes: sieve.primes.filter((value) => value >= start),
    crossedByBasePrime: sieve.crossedByBasePrime
  };
}

function factorizeCanonical(value) {
  const original = Math.abs(value);
  let current = original;
  const factors = [];

  if (current < 2) {
    return {
      value,
      factors: [],
      canonical: String(value),
      steps: [`Число ${value} не раскладывается на простые множители в стандартной форме.`]
    };
  }

  let divisor = 2;
  while (divisor * divisor <= current) {
    let power = 0;
    while (current % divisor === 0) {
      current /= divisor;
      power += 1;
    }

    if (power > 0) {
      factors.push({ prime: divisor, power });
    }

    divisor = divisor === 2 ? 3 : divisor + 2;
  }

  if (current > 1) {
    factors.push({ prime: current, power: 1 });
  }

  const steps = [];
  let remainder = original;
  for (const factor of factors) {
    for (let index = 0; index < factor.power; index++) {
      steps.push(`${remainder} / ${factor.prime} = ${remainder / factor.prime}`);
      remainder /= factor.prime;
    }
  }

  const canonical = factors
    .map((factor) =>
      factor.power === 1
        ? String(factor.prime)
        : `${factor.prime}^${factor.power}`
    )
    .join(" * ");

  return {
    value,
    factors,
    canonical,
    steps
  };
}

function explainPrimality(value) {
  if (!Number.isInteger(value) || value < 2) {
    return {
      value,
      isPrime: false,
      reason: "Число меньше 2 не является простым."
    };
  }

  if (value === 2) {
    return {
      value,
      isPrime: true,
      reason: "2 — простое число."
    };
  }

  if (value % 2 === 0) {
    return {
      value,
      isPrime: false,
      reason: `${value} делится на 2 без остатка.`
    };
  }

  const limit = Math.floor(Math.sqrt(value));
  for (let divisor = 3; divisor <= limit; divisor += 2) {
    if (value % divisor === 0) {
      return {
        value,
        isPrime: false,
        reason: `${value} делится на ${divisor}, значит число составное.`
      };
    }
  }

  return {
    value,
    isPrime: true,
    reason: `У числа ${value} нет делителей на отрезке [2, ${limit}], значит оно простое.`
  };
}

function buildGcdSteps(numbers) {
  const steps = [];

  if (numbers.length === 2) {
    let [a, b] = numbers.map((value) => Math.abs(value));
    if (a < b) {
      [a, b] = [b, a];
    }
    while (b !== 0) {
      const remainder = a % b;
      steps.push(`${a} = ${b} * ${Math.floor(a / b)} + ${remainder}`);
      a = b;
      b = remainder;
    }
    return steps;
  }

  const firstPairGcd = gcdTwo(numbers[0], numbers[1]);
  steps.push(`НОД(${numbers[0]}, ${numbers[1]}) = ${firstPairGcd}`);
  steps.push(`НОД(${firstPairGcd}, ${numbers[2]}) = ${gcdTwo(firstPairGcd, numbers[2])}`);
  return steps;
}

module.exports = {
  buildGcdSteps,
  explainPrimality,
  factorizeCanonical,
  findPrimesInRange,
  findPrimesWithSieve,
  gcdMany,
  gcdTwo,
  isPrime,
  normalizeNumbers,
  sievePrimes
};
