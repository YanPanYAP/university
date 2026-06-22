const express = require("express");
const app = express();
app.use(express.json());

let storedRequest = null;


function calculate(op, x, y) {
    switch (op) {
        case "add": return x + y;
        case "sub": return x - y;
        case "mul": return x * y;
        case "div": return y !== 0 ? x / y : 0;
        default: return null;
    }
}


app.get("/NGINX-test", (req, res) => {
    if (!storedRequest) {
        return res.status(404).send("JSON not found");
    }

    const result = calculate(storedRequest.op, storedRequest.x, storedRequest.y);
    res.json({ ...storedRequest, result });
});


app.post("/NGINX-test", (req, res) => {
    if (storedRequest) {
        return res.status(409).send("JSON already exists");
    }

    storedRequest = req.body;
    const result = calculate(req.body.op, req.body.x, req.body.y);
    res.json({ ...req.body, result });
});


app.put("/NGINX-test", (req, res) => {
    if (!storedRequest) {
        return res.status(404).send("JSON not found");
    }

    storedRequest = req.body;
    const result = calculate(req.body.op, req.body.x, req.body.y);
    res.json({ ...req.body, result });
});


app.delete("/NGINX-test", (req, res) => {
    if (!storedRequest) {
        return res.status(404).send("JSON not found");
    }

    storedRequest = null;
    res.status(200).send("Deleted");
});

app.listen(40000, () => {
    console.log("Server running on port 40000");
});
