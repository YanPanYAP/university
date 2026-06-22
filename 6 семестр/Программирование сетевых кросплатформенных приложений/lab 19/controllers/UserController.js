let users = [
    { id: 1, name: "Yan" },
    { id: 2, name: "Pan" }
];

module.exports = {

    getUsers: (req, res) => {
        res.json(users);
    },

    getUserById: (req, res) => {
        const user = users.find(u => u.id == req.params.id);
        if (!user) return res.status(404).send("User not found");
        res.json(user);
    },

    createUser: (req, res) => {
        const { name } = req.body;
        if (!name) return res.status(400).send("Name required");

        const newUser = { id: users.length + 1, name };
        users.push(newUser);

        res.status(201).json(newUser);
    },

    updateUser: (req, res) => {
        const user = users.find(u => u.id == req.params.id);
        if (!user) return res.status(404).send("User not found");

        user.name = req.body.name || user.name;
        res.json(user);
    },

    deleteUser: (req, res) => {
        const index = users.findIndex(u => u.id == req.params.id);
        if (index === -1) return res.status(404).send("User not found");

        users.splice(index, 1);
        res.send("User deleted");
    }
};