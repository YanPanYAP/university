const EventEmitter = require("events");

class DB extends EventEmitter {
  constructor() {
    super();

    this._rows = [
      { id: 1, name: "Yan Pavlovich", bday: "2006-02-17" },
      { id: 2, name: "Y Pan", bday: "2006-05-20" },
    ];
    this._nextId = 3;

    this.on("GET", async (ctx) => {
      setTimeout(() => ctx.resolve(this._rows.slice()), 50);
    });

    this.on("POST", async (ctx) => {
      setTimeout(() => {
        const row = ctx.data;
        const newRow = {
          id: this._nextId++,
          name: row.name || "",
          bday: row.bday || "",
        };
        this._rows.push(newRow);
        ctx.resolve(newRow);
      }, 50);
    });

    this.on("PUT", async (ctx) => {
      setTimeout(() => {
        const row = ctx.data;
        const idx = this._rows.findIndex((r) => r.id === row.id);
        if (idx === -1) return ctx.reject(new Error(`id=${row.id} не найден`));
        this._rows[idx] = { ...this._rows[idx], ...row };
        ctx.resolve(this._rows[idx]);
      }, 50);
    });

    this.on("DELETE", async (ctx) => {
      setTimeout(() => {
        const id = ctx.data;
        const idx = this._rows.findIndex((r) => r.id === id);
        if (idx === -1) return ctx.reject(new Error(`id=${id} не найден`));
        const deleted = this._rows.splice(idx, 1)[0];
        ctx.resolve(deleted);
      }, 50);
    });
  }

  select() {
    return new Promise((resolve, reject) =>
      this.emit("GET", { resolve, reject })
    );
  }

  insert(row) {
    return new Promise((resolve, reject) =>
      this.emit("POST", { data: row, resolve, reject })
    );
  }

  update(row) {
    return new Promise((resolve, reject) =>
      this.emit("PUT", { data: row, resolve, reject })
    );
  }

  delete(id) {
    return new Promise((resolve, reject) =>
      this.emit("DELETE", { data: id, resolve, reject })
    );
  }
}

module.exports = DB;
