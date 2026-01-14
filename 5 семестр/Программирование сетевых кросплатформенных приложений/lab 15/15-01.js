const http = require('http');
const MongoClient = require('mongodb').MongoClient;

const PORT = 3000;
const url = 'mongodb+srv://student:student123@bstu.d0swjpk.mongodb.net/?appName=BSTU';
const client = new MongoClient(url);
const dbName = 'BSTU';

let db, faculty, pulpit;

async function init() {
    await client.connect();
    db = client.db(dbName);
    faculty = db.collection('faculty');
    pulpit = db.collection('pulpit');
    console.log('MongoDB connected');
}
init();

let sendJSON = (res, status, data) => {
    res.writeHead(status, {'Content-Type': 'application/json; charset=utf-8'});
    res.end(JSON.stringify(data));
};

let parseBody = (req) => new Promise((res, rej) => {
    let body = '';
    req.on('data', chunk => body += chunk);
    req.on('end', () => {
        try { res(JSON.parse(body || '{}')); }
        catch (err) { rej(new Error('Invalid JSON')); }
    });
});

let server = http.createServer(async (req, res) => {
    let url = req.url;
    let method = req.method;

    if (method === 'GET') {
        if (url === '/api/faculties') {
            try {
                let result = await faculty.find().toArray();
                return sendJSON(res, 200, result);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }
        if (url === '/api/pulpits') {
            try {
                let result = await pulpit.find().toArray();
                return sendJSON(res, 200, result);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }
    }

    else if (method === 'POST') {
        if (url === '/api/faculties') {
            try {
                let data = await parseBody(req);

                if (!data.faculty || !data.faculty_name) {
                    return sendJSON(res, 400, { error: 'Missing required fields: faculty, faculty_name' });
                }

                let exists = await faculty.findOne({ faculty: data.faculty });
                if (exists) {
                    return sendJSON(res, 400, { error: `Faculty ${data.faculty} already exists` });
                }

                await faculty.insertOne(data);
                return sendJSON(res, 200, data);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }

        if (url === '/api/pulpits') {
            try {
                let data = await parseBody(req);

                if (!data.pulpit || !data.pulpit_name || !data.faculty) {
                    return sendJSON(res, 400, { error: 'Missing required fields: pulpit, pulpit_name, faculty' });
                }

                let fac = await faculty.findOne({ faculty: data.faculty });
                if (!fac) {
                    return sendJSON(res, 400, { error: `Faculty ${data.faculty} does not exist` });
                }

                let exists = await pulpit.findOne({ pulpit: data.pulpit });
                if (exists) {
                    return sendJSON(res, 400, { error: `Pulpit ${data.pulpit} already exists` });
                }

                await pulpit.insertOne(data);
                return sendJSON(res, 200, data);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }
    }

    else if (method === 'PUT') {
        if (url === '/api/faculties') {
            try {
                let data = await parseBody(req);

                if (!data.faculty || !data.faculty_name) {
                    return sendJSON(res, 400, { error: 'Missing required fields: faculty, faculty_name' });
                }

                let result = await faculty.updateOne(
                    { faculty: data.faculty },
                    { $set: { faculty_name: data.faculty_name } }
                );

                if (result.matchedCount === 0) {
                    return sendJSON(res, 404, { error: `Faculty ${data.faculty} not found` });
                }

                return sendJSON(res, 200, data);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }

        if (url === '/api/pulpits') {
            try {
                let data = await parseBody(req);

                if (!data.pulpit || !data.pulpit_name || !data.faculty) {
                    return sendJSON(res, 400, { error: 'Missing required fields: pulpit, pulpit_name, faculty' });
                }

                let fac = await faculty.findOne({ faculty: data.faculty });
                if (!fac) {
                    return sendJSON(res, 400, { error: `Faculty ${data.faculty} does not exist` });
                }

                let result = await pulpit.updateOne(
                    { pulpit: data.pulpit },
                    { $set: { pulpit_name: data.pulpit_name, faculty: data.faculty } }
                );

                if (result.matchedCount === 0) {
                    return sendJSON(res, 404, { error: `Pulpit ${data.pulpit} not found` });
                }

                return sendJSON(res, 200, data);
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }
    }

    else if (method === 'DELETE') {
        let m = url.match(/^\/api\/(faculties|pulpits)\/(.+)$/);
        if (m) {
            let entity = m[1];
            let code = decodeURIComponent(m[2]);

            try {
                if (entity === 'faculties') {

                    let findblock = await pulpit.findOne({ faculty: code });
                    if (findblock) {
                        return sendJSON(res, 400, { error: 'Cannot delete faculty: related pulpit exists' });
                    }

                    let result = await faculty.deleteOne({ faculty: code });
                    if (result.deletedCount === 0) {
                        return sendJSON(res, 404, { error: `Faculty ${code} not found` });
                    }

                    return sendJSON(res, 200, { deleted: code });
                }

                if (entity === 'pulpits') {
                    let result = await pulpit.deleteOne({ pulpit: code });
                    if (result.deletedCount === 0) {
                        return sendJSON(res, 404, { error: `Pulpit ${code} not found` });
                    }

                    return sendJSON(res, 200, { deleted: code });
                }
            } catch (e) {
                return sendJSON(res, 500, { error: e.message });
            }
        }
    }

    return sendJSON(res, 404, { error: 'Route not found' });
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}`);
