const http = require('http');
const { pool } = require('./db');
const fs = require('fs');

const PORT = 3000;

const sendJSON = (res, status, data) => {
    res.writeHead(status, { 'Content-Type': 'application/json; charset=utf-8' });
    res.end(JSON.stringify(data));
};

const sendHTML = (res, htmlBuffer) => {
    res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
    res.end(htmlBuffer);
};

const parseBody = (req) => new Promise((resolve, reject) => {
    let body = '';
    req.on('data', chunk => body += chunk);
    req.on('end', () => {
        try { resolve(JSON.parse(body || '{}')); }
        catch (err) { reject({ type: 'invalid_json', message: 'Invalid JSON' }); }
    });
});

const routMap = {
    faculties: { table: 'FACULTY', key: 'FACULTY' },
    pulpits: { table: 'PULPIT', key: 'PULPIT', fk: { column: 'FACULTY', table: 'FACULTY' } },
    subjects: { table: 'SUBJECT', key: 'SUBJECT', fk: { column: 'PULPIT', table: 'PULPIT' } },
    teachers: { table: 'TEACHER', key: 'TEACHER', fk: { column: 'PULPIT', table: 'PULPIT' } },
    auditoriumstypes: { table: 'AUDITORIUM_TYPE', key: 'AUDITORIUM_TYPE' },
    auditoriums: { table: 'AUDITORIUM', key: 'AUDITORIUM', fk: { column: 'AUDITORIUM_TYPE', table: 'AUDITORIUM_TYPE' } },
};

const server = http.createServer(async (req, res) => {
    try {
        const url = req.url;
        const method = req.method;

        if (method === 'GET' && url === '/') {
            try {
                const html = fs.readFileSync('./index.html');
                return sendHTML(res, html);
            } catch (e) {
                return sendJSON(res, 500, { error: 'index.html not found' });
            }
        }

        if (method === 'GET') {
            const m = url.match(/^\/api\/(\w+)$/);
            if (m) {
                const entity = m[1];
                const route = routMap[entity];
                if (!route) return sendJSON(res, 404, { error: 'Unknown entity' });

                try {
                    const [rows] = await pool.query(`SELECT * FROM ${route.table}`);
                    return sendJSON(res, 200, rows);
                } catch (e) {
                    return sendJSON(res, 500, { error: e.message });
                }
            }
        }

        if (method === 'POST') {
            const m = url.match(/^\/api\/(\w+)$/);
            if (m) {
                const entity = m[1];
                const route = routMap[entity];
                if (!route) return sendJSON(res, 404, { error: 'Unknown entity' });

                try {
                    const data = await parseBody(req);

                    if (route.fk) {
                        const fkValue = data[route.fk.column];
                        if (!fkValue) return sendJSON(res, 400, { error: `Missing foreign key: ${route.fk.column}` });
                        const [fkRows] = await pool.query(`SELECT 1 FROM ${route.fk.table} WHERE ${route.fk.column} = ?`, [fkValue]);
                        if (!fkRows.length) return sendJSON(res, 400, { error: `Foreign key constraint failed: ${route.fk.column}='${fkValue}' not found` });
                    }

                    let sqlQuery, params;
                    switch (entity) {
                        case 'faculties':
                            if (!data.FACULTY || !data.FACULTY_NAME) throw { type: 'bad_request', message: 'Missing FACULTY or FACULTY_NAME' };
                            sqlQuery = 'INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES (?, ?)';
                            params = [data.FACULTY, data.FACULTY_NAME];
                            break;
                        case 'pulpits':
                            if (!data.PULPIT || !data.PULPIT_NAME || !data.FACULTY) throw { type: 'bad_request', message: 'Missing PULPIT, PULPIT_NAME, or FACULTY' };
                            sqlQuery = 'INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES (?, ?, ?)';
                            params = [data.PULPIT, data.PULPIT_NAME, data.FACULTY];
                            break;
                        case 'subjects':
                            if (!data.SUBJECT || !data.SUBJECT_NAME || !data.PULPIT) throw { type: 'bad_request', message: 'Missing SUBJECT, SUBJECT_NAME, or PULPIT' };
                            sqlQuery = 'INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES (?, ?, ?)';
                            params = [data.SUBJECT, data.SUBJECT_NAME, data.PULPIT];
                            break;
                        case 'teachers':
                            if (!data.TEACHER || !data.TEACHER_NAME || !data.PULPIT) throw { type: 'bad_request', message: 'Missing TEACHER, TEACHER_NAME, or PULPIT' };
                            sqlQuery = 'INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT) VALUES (?, ?, ?)';
                            params = [data.TEACHER, data.TEACHER_NAME, data.PULPIT];
                            break;
                        case 'auditoriumstypes':
                            if (!data.AUDITORIUM_TYPE || !data.AUDITORIUM_TYPENAME) throw { type: 'bad_request', message: 'Missing AUDITORIUM_TYPE or AUDITORIUM_TYPENAME' };
                            sqlQuery = 'INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES (?, ?)';
                            params = [data.AUDITORIUM_TYPE, data.AUDITORIUM_TYPENAME];
                            break;
                        case 'auditoriums':
                            if (!data.AUDITORIUM || !data.AUDITORIUM_TYPE) throw { type: 'bad_request', message: 'Missing AUDITORIUM or AUDITORIUM_TYPE' };
                            sqlQuery = 'INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY, AUDITORIUM_NAME) VALUES (?, ?, ?, ?)';
                            params = [data.AUDITORIUM, data.AUDITORIUM_TYPE, data.AUDITORIUM_CAPACITY || 0, data.AUDITORIUM_NAME || null];
                            break;
                        default:
                            return sendJSON(res, 404, { error: 'Unknown entity' });
                    }

                    await pool.query(sqlQuery, params);
                    return sendJSON(res, 201, data);

                } catch (e) {
                    if (e.type === 'bad_request' || e.type === 'invalid_json') return sendJSON(res, 400, { error: e.message });
                    if (e.code === 'ER_DUP_ENTRY') return sendJSON(res, 400, { error: 'Duplicate entry' });
                    if (e.code === 'ER_NO_REFERENCED_ROW_2') return sendJSON(res, 400, { error: 'Foreign key constraint failed' });
                    return sendJSON(res, 500, { error: e.message });
                }
            }
        }

        if (method === 'PUT') {
            const m = url.match(/^\/api\/(\w+)$/);
            if (m) {
                const entity = m[1];
                const route = routMap[entity];
                if (!route) return sendJSON(res, 404, { error: 'Unknown entity' });

                try {
                    const data = await parseBody(req);

                    if (route.fk) {
                        const fkValue = data[route.fk.column];
                        if (!fkValue) return sendJSON(res, 400, { error: `Missing foreign key: ${route.fk.column}` });
                        const [fkRows] = await pool.query(`SELECT 1 FROM ${route.fk.table} WHERE ${route.fk.column} = ?`, [fkValue]);
                        if (!fkRows.length) return sendJSON(res, 400, { error: `Foreign key constraint failed: ${route.fk.column}='${fkValue}' not found` });
                    }

                    let sqlQuery, params;
                    switch (entity) {
                        case 'faculties':
                            sqlQuery = 'UPDATE FACULTY SET FACULTY_NAME = ? WHERE FACULTY = ?';
                            params = [data.FACULTY_NAME, data.FACULTY];
                            break;
                        case 'pulpits':
                            sqlQuery = 'UPDATE PULPIT SET PULPIT_NAME = ?, FACULTY = ? WHERE PULPIT = ?';
                            params = [data.PULPIT_NAME, data.FACULTY, data.PULPIT];
                            break;
                        case 'subjects':
                            sqlQuery = 'UPDATE SUBJECT SET SUBJECT_NAME = ?, PULPIT = ? WHERE SUBJECT = ?';
                            params = [data.SUBJECT_NAME, data.PULPIT, data.SUBJECT];
                            break;
                        case 'teachers':
                            sqlQuery = 'UPDATE TEACHER SET TEACHER_NAME = ?, PULPIT = ? WHERE TEACHER = ?';
                            params = [data.TEACHER_NAME, data.PULPIT, data.TEACHER];
                            break;
                        case 'auditoriumstypes':
                            sqlQuery = 'UPDATE AUDITORIUM_TYPE SET AUDITORIUM_TYPENAME = ? WHERE AUDITORIUM_TYPE = ?';
                            params = [data.AUDITORIUM_TYPENAME, data.AUDITORIUM_TYPE];
                            break;
                        case 'auditoriums':
                            sqlQuery = 'UPDATE AUDITORIUM SET AUDITORIUM_TYPE = ?, AUDITORIUM_CAPACITY = ?, AUDITORIUM_NAME = ? WHERE AUDITORIUM = ?';
                            params = [data.AUDITORIUM_TYPE, data.AUDITORIUM_CAPACITY, data.AUDITORIUM_NAME, data.AUDITORIUM];
                            break;
                        default:
                            return sendJSON(res, 404, { error: 'Unknown entity' });
                    }

                    await pool.query(sqlQuery, params);
                    return sendJSON(res, 200, data);
                } catch (e) {
                    if (e.type === 'bad_request' || e.type === 'invalid_json') return sendJSON(res, 400, { error: e.message });
                    if (e.code === 'ER_NO_REFERENCED_ROW_2') return sendJSON(res, 400, { error: 'Foreign key constraint failed' });
                    return sendJSON(res, 500, { error: e.message });
                }
            }
        }

        if (method === 'DELETE') {
            const m = url.match(/^\/api\/([^\/]+)\/(.+)$/);
            if (m) {
                const entity = m[1];
                const id = decodeURIComponent(m[2]);
                const route = routMap[entity];
                if (!route) return sendJSON(res, 404, { error: 'Unknown entity' });

                try {
                    const [rows] = await pool.query(`SELECT * FROM ${route.table} WHERE ${route.key} = ?`, [id]);
                    if (!rows.length) return sendJSON(res, 404, { error: 'Record not found' });

                    await pool.query(`DELETE FROM ${route.table} WHERE ${route.key} = ?`, [id]);
                    return sendJSON(res, 200, { deleted: id });
                } catch (e) {
                    if (e.code === 'ER_ROW_IS_REFERENCED_2') return sendJSON(res, 400, { error: 'Cannot delete: record is referenced by another table' });
                    return sendJSON(res, 500, { error: e.message });
                }
            }
        }

        return sendJSON(res, 404, { error: 'Route not found' });

    } catch (e) {
        return sendJSON(res, 500, { error: e.message });
    }
});

server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
