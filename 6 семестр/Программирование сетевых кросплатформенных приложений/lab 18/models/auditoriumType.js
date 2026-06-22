const { DataTypes } = require('sequelize');
const sequelize = require('../db');

module.exports = sequelize.define('AUDITORIUM_TYPE', {
    AUDITORIUM_TYPE: { type: DataTypes.STRING(10), primaryKey: true },
    AUDITORIUM_TYPENAME: DataTypes.STRING(30)
}, {
    tableName: 'AUDITORIUM_TYPE',
    timestamps: false
});