const express = require('express');
const bodyParser = require('body-parser');

const routing = require('./routing');
const controllers = require('./controllers');

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

function matchRoute(req) {
    for (let route of routing) {

        if (route.method !== req.method) continue;

        const routeParts = route.path.split('/');
        const urlParts = req.path.split('/');

        if (routeParts.length !== urlParts.length) continue;

        let params = {};
        let isMatch = true;

        for (let i = 0; i < routeParts.length; i++) {
            if (routeParts[i].startsWith(':')) {
                const paramName = routeParts[i].substring(1);
                params[paramName] = urlParts[i];
            } else if (routeParts[i] !== urlParts[i]) {
                isMatch = false;
                break;
            }
        }

        if (isMatch) {
            return { route, params };
        }
    }

    return null;
}

app.use((req, res) => {

    const result = matchRoute(req);

    if (!result) {
        return res.status(404).send("Route not found");
    }

    const controller = controllers[result.route.controller];
    const action = controller[result.route.action];

    req.params = result.params;

    action(req, res);
});

app.listen(3000, () => {
    console.log("Server started on http://localhost:3000");
});