const http = require('http');
const { graphql, buildSchema } = require('graphql');
const { pool } = require('./db');

let schemaSDL = `
    schema{
        query: Query
        mutation: Mutation
    }

    type Faculty{
        faculty: String!
        faculty_name: String!
    }
    type Pulpit{
        pulpit: String!
        pulpit_name: String!
        faculty: String!
    }
    type Teacher{
        teacher: String!
        teacher_name: String!
        pulpit: String!
        faculty: String
    }
    type Subject{
        subject: String!
        subject_name: String!
        pulpit: String!
    }
    type PulpitWithSubjects{
        pulpit: String!
        pulpit_name: String!
        subjects: [Subject!]
        faculty: String
    }

    type Query{
        getFaculties(faculty: String): [Faculty!]
        getTeachers(teacher: String): [Teacher!]
        getPulpits(pulpit: String): [Pulpit!]
        getSubjects(subject: String): [Subject!]

        getTeachersByFaculty(faculty: String!): [Teacher!]!
        getSubjectsByFaculties(faculty: String!): [PulpitWithSubjects!]!
    }

    input FacultyInput{
        faculty: String!
        faculty_name: String!
    }
    input TeacherInput{
        teacher: String!
        teacher_name: String!
        pulpit: String!
    }
    input PulpitInput{
        pulpit: String!
        pulpit_name: String!
        faculty: String!
    }
    input SubjectInput{
        subject: String!
        subject_name: String!
        pulpit: String!
    }

    type Mutation{
        setFaculty(faculty: FacultyInput!): Faculty!
        setTeacher(teacher: TeacherInput!): Teacher!
        setPulpit(pulpit: PulpitInput!): Pulpit!
        setSubject(subject: SubjectInput!): Subject!

        delFaculty(faculty: String!): Boolean!
        delTeacher(teacher: String!): Boolean!
        delPulpit(pulpit: String!): Boolean!
        delSubject(subject: String!): Boolean!
    }
`;

let schema = buildSchema(schemaSDL);

let root = {
    getFaculties: async ({ faculty }) => {
        if (faculty) {
            const [rows] = await pool.query(
                'SELECT FACULTY as faculty, FACULTY_NAME as faculty_name FROM FACULTY WHERE FACULTY = ?',
                [faculty]
            );
            return rows;
        } else {
            const [rows] = await pool.query(
                'SELECT FACULTY as faculty, FACULTY_NAME as faculty_name FROM FACULTY'
            );
            return rows;
        }
    },

    getTeachers: async ({ teacher }) => {
        if (teacher) {
            const [rows] = await pool.query(
                'SELECT TEACHER as teacher, TEACHER_NAME as teacher_name, PULPIT as pulpit FROM TEACHER WHERE TEACHER = ?',
                [teacher]
            );
            return rows;
        } else {
            const [rows] = await pool.query(
                'SELECT TEACHER as teacher, TEACHER_NAME as teacher_name, PULPIT as pulpit FROM TEACHER'
            );
            return rows;
        }
    },

    getPulpits: async ({ pulpit }) => {
        if (pulpit) {
            const [rows] = await pool.query(
                'SELECT PULPIT as pulpit, PULPIT_NAME as pulpit_name, FACULTY as faculty FROM PULPIT WHERE PULPIT = ?',
                [pulpit]
            );
            return rows;
        } else {
            const [rows] = await pool.query(
                'SELECT PULPIT as pulpit, PULPIT_NAME as pulpit_name, FACULTY as faculty FROM PULPIT'
            );
            return rows;
        }
    },

    getSubjects: async ({ subject }) => {
        if (subject) {
            const [rows] = await pool.query(
                'SELECT SUBJECT as subject, SUBJECT_NAME as subject_name, PULPIT as pulpit FROM SUBJECT WHERE SUBJECT = ?',
                [subject]
            );
            return rows;
        } else {
            const [rows] = await pool.query(
                'SELECT SUBJECT as subject, SUBJECT_NAME as subject_name, PULPIT as pulpit FROM SUBJECT'
            );
            return rows;
        }
    },

    getTeachersByFaculty: async ({ faculty }) => {
        const [rows] = await pool.query(
            `SELECT t.TEACHER as teacher, t.TEACHER_NAME as teacher_name, t.PULPIT as pulpit, p.FACULTY as faculty
             FROM TEACHER t
             JOIN PULPIT p ON p.PULPIT = t.PULPIT
             WHERE p.FACULTY = ?`,
            [faculty]
        );
        return rows;
    },

    getSubjectsByFaculties: async ({ faculty }) => {
        const [rows] = await pool.query(
            `SELECT p.PULPIT as pulpit, p.PULPIT_NAME as pulpit_name, p.FACULTY as faculty,
                    s.PULPIT as s_pulpit, s.SUBJECT as subject, s.SUBJECT_NAME as subject_name
             FROM PULPIT p
             LEFT JOIN SUBJECT s ON s.PULPIT = p.PULPIT
             WHERE p.FACULTY = ?
             ORDER BY p.PULPIT`,
            [faculty]
        );

        let map = new Map();
        for (let row of rows) {
            if (!map.has(row.pulpit)) {
                map.set(row.pulpit, { pulpit: row.pulpit, pulpit_name: row.pulpit_name, faculty: row.faculty, subjects: [] });
            }
            if (row.subject) {
                map.get(row.pulpit).subjects.push({
                    subject: row.subject,
                    subject_name: row.subject_name,
                    pulpit: row.s_pulpit
                });
            }
        }
        return Array.from(map.values());
    },

    setFaculty: async ({ faculty }) => {
        await pool.query(
            `INSERT INTO FACULTY (FACULTY, FACULTY_NAME)
             VALUES (?, ?)
             ON DUPLICATE KEY UPDATE FACULTY_NAME = VALUES(FACULTY_NAME)`,
            [faculty.faculty, faculty.faculty_name]
        );
        return faculty;
    },

    setPulpit: async ({ pulpit }) => {
        await pool.query(
            `INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
             VALUES (?, ?, ?)
             ON DUPLICATE KEY UPDATE PULPIT_NAME = VALUES(PULPIT_NAME), FACULTY = VALUES(FACULTY)`,
            [pulpit.pulpit, pulpit.pulpit_name, pulpit.faculty]
        );
        return pulpit;
    },

    setTeacher: async ({ teacher }) => {
        await pool.query(
            `INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
             VALUES (?, ?, ?)
             ON DUPLICATE KEY UPDATE TEACHER_NAME = VALUES(TEACHER_NAME), PULPIT = VALUES(PULPIT)`,
            [teacher.teacher, teacher.teacher_name, teacher.pulpit]
        );
        return teacher;
    },

    setSubject: async ({ subject }) => {
        await pool.query(
            `INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
             VALUES (?, ?, ?)
             ON DUPLICATE KEY UPDATE SUBJECT_NAME = VALUES(SUBJECT_NAME), PULPIT = VALUES(PULPIT)`,
            [subject.subject, subject.subject_name, subject.pulpit]
        );
        return subject;
    },

    delFaculty: async ({ faculty }) => {
        const [hasPulpits] = await pool.query('SELECT 1 FROM PULPIT WHERE FACULTY = ? LIMIT 1', [faculty]);
        if (hasPulpits.length) return false;

        const [result] = await pool.query('DELETE FROM FACULTY WHERE FACULTY = ?', [faculty]);
        return result.affectedRows > 0;
    },

    delPulpit: async ({ pulpit }) => {
        const [hasTeachers] = await pool.query('SELECT 1 FROM TEACHER WHERE PULPIT = ? LIMIT 1', [pulpit]);
        if (hasTeachers.length) return false;

        const [hasSubjects] = await pool.query('SELECT 1 FROM SUBJECT WHERE PULPIT = ? LIMIT 1', [pulpit]);
        if (hasSubjects.length) return false;

        const [result] = await pool.query('DELETE FROM PULPIT WHERE PULPIT = ?', [pulpit]);
        return result.affectedRows > 0;
    },

    delTeacher: async ({ teacher }) => {
        const [result] = await pool.query('DELETE FROM TEACHER WHERE TEACHER = ?', [teacher]);
        return result.affectedRows > 0;
    },

    delSubject: async ({ subject }) => {
        const [result] = await pool.query('DELETE FROM SUBJECT WHERE SUBJECT = ?', [subject]);
        return result.affectedRows > 0;
    }
};

let server = http.createServer(async (req, res) => {
    if (req.method === 'POST' && req.url === '/graphql') {
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', async () => {
            try {
                let { query, variables } = JSON.parse(body || '{}');
                let result = await graphql({
                    schema,
                    source: query,
                    rootValue: root,
                    variableValues: variables
                });
                res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
                res.end(JSON.stringify(result));
            } catch (e) {
                res.writeHead(400, { 'Content-Type': 'application/json; charset=utf-8' });
                res.end(JSON.stringify({ errors: [{ message: e.message }] }));
            }
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ error: 'Route not found' }));
    }
}).listen(3000);

console.log('Server running at http://localhost:3000/graphql');
