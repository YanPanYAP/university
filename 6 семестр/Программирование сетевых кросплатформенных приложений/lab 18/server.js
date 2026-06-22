const express = require('express');
const sequelize = require('./db');

const Faculty = require('./models/faculty');
const Pulpit = require('./models/pulpit');
const Subject = require('./models/subject');
const AuditoriumType = require('./models/auditoriumType');
const Auditorium = require('./models/auditorium');

const app = express();
app.use(express.json());
app.use(express.static('static'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/static/index.html');
});



app.get('/api/faculties', async (req,res)=> res.json(await Faculty.findAll()));
app.get('/api/pulpits', async (req,res)=> res.json(await Pulpit.findAll()));
app.get('/api/subjects', async (req,res)=> res.json(await Subject.findAll()));
app.get('/api/auditoriumstypes', async (req,res)=> res.json(await AuditoriumType.findAll()));
app.get('/api/auditoriums', async (req,res)=> res.json(await Auditorium.findAll()));



app.post('/api/faculties', async (req,res)=>{
    try { res.json(await Faculty.create(req.body)); }
    catch(e){ res.status(400).json({error:e.message}); }
});

app.post('/api/pulpits', async (req,res)=>{
    try { res.json(await Pulpit.create(req.body)); }
    catch(e){ res.status(400).json({error:e.message}); }
});

app.post('/api/subjects', async (req,res)=>{
    try { res.json(await Subject.create(req.body)); }
    catch(e){ res.status(400).json({error:e.message}); }
});

app.post('/api/auditoriumstypes', async (req,res)=>{
    try { res.json(await AuditoriumType.create(req.body)); }
    catch(e){ res.status(400).json({error:e.message}); }
});

app.post('/api/auditoriums', async (req,res)=>{
    try { res.json(await Auditorium.create(req.body)); }
    catch(e){ res.status(400).json({error:e.message}); }
});



app.put('/api/faculties', async (req,res)=>{
    try {
        await Faculty.update(req.body, {where:{FACULTY:req.body.FACULTY}});
        res.json(await Faculty.findByPk(req.body.FACULTY));
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.put('/api/pulpits', async (req,res)=>{
    try {
        await Pulpit.update(req.body,{where:{PULPIT:req.body.PULPIT}});
        res.json(await Pulpit.findByPk(req.body.PULPIT));
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.put('/api/subjects', async (req,res)=>{
    try {
        await Subject.update(req.body,{where:{SUBJECT:req.body.SUBJECT}});
        res.json(await Subject.findByPk(req.body.SUBJECT));
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.put('/api/auditoriumstypes', async (req,res)=>{
    try {
        await AuditoriumType.update(req.body,{where:{AUDITORIUM_TYPE:req.body.AUDITORIUM_TYPE}});
        res.json(await AuditoriumType.findByPk(req.body.AUDITORIUM_TYPE));
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.put('/api/auditoriums', async (req,res)=>{
    try {
        await Auditorium.update(req.body,{where:{AUDITORIUM:req.body.AUDITORIUM}});
        res.json(await Auditorium.findByPk(req.body.AUDITORIUM));
    } catch(e){ res.status(400).json({error:e.message}); }
});



app.delete('/api/faculties/:id', async (req,res)=>{
    try {
        const item = await Faculty.findByPk(req.params.id);
        await item.destroy();
        res.json(item);
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.delete('/api/pulpits/:id', async (req,res)=>{
    try {
        const item = await Pulpit.findByPk(req.params.id);
        await item.destroy();
        res.json(item);
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.delete('/api/subjects/:id', async (req,res)=>{
    try {
        const item = await Subject.findByPk(req.params.id);
        await item.destroy();
        res.json(item);
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.delete('/api/auditoriumtypes/:id', async (req,res)=>{
    try {
        const item = await AuditoriumType.findByPk(req.params.id);
        await item.destroy();
        res.json(item);
    } catch(e){ res.status(400).json({error:e.message}); }
});

app.delete('/api/auditoriums/:id', async (req,res)=>{
    try {
        const item = await Auditorium.findByPk(req.params.id);
        await item.destroy();
        res.json(item);
    } catch(e){ res.status(400).json({error:e.message}); }
});

sequelize.authenticate()
.then(()=>console.log("DB connected"))
.catch(err=>console.log(err));

app.listen(3000, ()=>console.log("Server started on port 3000"));