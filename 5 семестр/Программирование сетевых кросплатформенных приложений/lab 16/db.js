const mysql = require('mysql2/promise');

let pool = mysql.createPool({
    host: 'localhost',
    user: 'student',
    password: 'fitfit',
    database: 'YAP',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

pool.getConnection()
    .then(conn => {
        console.log('MySQL connected');
        conn.release(); 
    })
    .catch(err => {
        console.error('MySQL connection error: ' + err.message);
        throw err;
    });

module.exports = { pool };
