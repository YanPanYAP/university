const { DataTypes } = require('sequelize');
const sequelize = require('../db');

module.exports = sequelize.define('PULPIT', {
    PULPIT: { type: DataTypes.STRING(20), primaryKey: true },
    PULPIT_NAME: DataTypes.STRING(100),
    FACULTY: DataTypes.STRING(10)
}, {
    tableName: 'PULPIT',
    timestamps: false
});