const { DataTypes } = require('sequelize');
const sequelize = require('../db');

module.exports = sequelize.define('SUBJECT', {
    SUBJECT: { type: DataTypes.STRING(10), primaryKey: true },
    SUBJECT_NAME: DataTypes.STRING(100),
    PULPIT: DataTypes.STRING(20)
}, {
    tableName: 'SUBJECT',
    timestamps: false
});