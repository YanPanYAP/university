//задание 1
console.log("задание 1\n");

abstract class BaseUser {
  constructor(protected id: number, protected name: string) {}
  abstract getRole(): string;
  abstract getPermissions(): string[];
}

class Guest extends BaseUser {
  getRole(): string {
    return "Гость";
  }
  getPermissions(): string[] {
    return ["Просмотр контента"];
  }
}

class User extends BaseUser {
  getRole(): string {
    return "Пользователь";
  }
  getPermissions(): string[] {
    return ["Просмотр контента", "Добавление комментариев"];
  }
}

class Admin extends BaseUser {
  getRole(): string {
    return "Администратор";
  }
  getPermissions(): string[] {
    return [
      "Просмотр контента",
      "Добавление комментариев",
      "Удаление комментариев",
      "Управление пользователями",
    ];
  }
}

const guest = new Guest(1, "Аноним");
console.log(guest.getPermissions());

const admin = new Admin(2, "Мария");
console.log(admin.getPermissions());

//задание 2
console.log("\nзадание 2\n");

interface IReport {
  title: string;
  content: string;
  generate(): string | object;
}

class HTMLReport implements IReport {
  constructor(public title: string, public content: string) {}
  generate(): string {
    return `<h1>${this.title}</h1><p>${this.content}</p>`;
  }
}

class JSONReport implements IReport {
  constructor(public title: string, public content: string) {}
  generate(): object {
    return { title: this.title, content: this.content };
  }
}

const report1 = new HTMLReport("Отчет 1", "Содержание отчета");
console.log(report1.generate());

const report2 = new JSONReport("Отчет 2", "Содержание отчета");
console.log(report2.generate());

//задание 3
console.log("\nзадание 3\n");

class Cache2<T> {
  private storage = new Map<string, { value: T; expires: number }>();

  add(key: string, value: T, ttl: number): void {
    const expires = Date.now() + ttl;
    this.storage.set(key, { value, expires });
  }

  get(key: string): T | null {
    const entry = this.storage.get(key);
    if (!entry || Date.now() > entry.expires) {
      this.storage.delete(key);
      return null;
    }
    return entry.value;
  }

  clearExpired(): void {
    this.storage.forEach((entry, key) => {
      if (Date.now() > entry.expires) {
        this.storage.delete(key);
      }
    });
  }
}

const cache = new Cache2<number>();
cache.add("price", 100, 5000);
console.log(cache.get("price"));
setTimeout(() => console.log(cache.get("price")), 6000);

//задание 4
console.log("\nзадание 4\n");

function createInstance<T>(cls: new (...args: any[]) => T, ...args: any[]): T {
  return new cls(...args);
}

class Product {
  constructor(public name: string, public price: number) {}
}

const p = createInstance(Product, "Телефон", 50000);
console.log(p);

//задание 5
console.log("\nзадание 5\n");

enum LogLevel {
  INFO = "INFO",
  WARNING = "WARNING",
  ERROR = "ERROR",
}

type LogEntry = [Date, LogLevel, string];

function logEvent(event: LogEntry): void {
  console.log(`[${event[0].toISOString()}] [${event[1]}]: ${event[2]}`);
}

logEvent([new Date(), LogLevel.INFO, "Система запущена"]);

//задание 6
console.log("\nзадание 6\n");

enum HttpStatus {
  OK = 200,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  INTERNAL_ERROR = 500,
}

type ApiResponse<T> = [HttpStatus, T | null, string?];

function success<T>(data: T): ApiResponse<T> {
  return [HttpStatus.OK, data];
}

function error(message: string, status: HttpStatus): ApiResponse<null> {
  return [status, null, message];
}

const res1 = success({ user: "Андрей" });
console.log(res1);

const res2 = error("Не найдено", HttpStatus.BAD_REQUEST);
console.log(res2);
