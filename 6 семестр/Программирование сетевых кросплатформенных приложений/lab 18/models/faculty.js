const { DataTypes } = require('sequelize');
const sequelize = require('../db');

module.exports = sequelize.define('FACULTY', {
    FACULTY: { type: DataTypes.STRING(10), primaryKey: true },
    FACULTY_NAME: { type: DataTypes.STRING(50) }
}, {
    tableName: 'FACULTY',
    timestamps: false
});