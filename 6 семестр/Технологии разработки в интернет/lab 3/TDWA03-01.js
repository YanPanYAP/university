const url = "https://localhost:20443/api/Save-JSON";

function getInput() {
    return {
        op: document.getElementById("op").value,
        x: Number(document.getElementById("x").value),
        y: Number(document.getElementById("y").value)
    };
}

function showResult(data) {
    document.getElementById("result").textContent =
        typeof data === "string" ? data : JSON.stringify(data, null, 2);
}

async function sendGet() {
    const res = await fetch(url);
    const text = await res.text();
    showResult(text);
}

async function sendPost() {
    const res = await fetch(url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(getInput())
    });
    const data = await res.json();
    showResult(data);
}

async function sendPut() {
    const res = await fetch(url, {
        method: "PUT",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(getInput())
    });
    const data = await res.json();
    showResult(data);
}

async function sendDelete() {
    const res = await fetch(url, { method: "DELETE" });
    const text = await res.text();
    showResult(text);
}
