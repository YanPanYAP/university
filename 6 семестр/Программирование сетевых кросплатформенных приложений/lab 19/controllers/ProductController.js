let products = [
    { id: 1, title: "Mac" },
    { id: 2, title: "iPhone" }
];

module.exports = {

    getProducts: (req, res) => {
        res.json(products);
    },

    getProductById: (req, res) => {
        const product = products.find(p => p.id == req.params.id);
        if (!product) return res.status(404).send("Product not found");
        res.json(product);
    },

    createProduct: (req, res) => {
        const { title } = req.body;
        if (!title) return res.status(400).send("Title required");

        const newProduct = { id: products.length + 1, title };
        products.push(newProduct);

        res.status(201).json(newProduct);
    },

    updateProduct: (req, res) => {
        const product = products.find(p => p.id == req.params.id);
        if (!product) return res.status(404).send("Product not found");

        product.title = req.body.title || product.title;
        res.json(product);
    },

    deleteProduct: (req, res) => {
        const index = products.findIndex(p => p.id == req.params.id);
        if (index === -1) return res.status(404).send("Product not found");

        products.splice(index, 1);
        res.send("Product deleted");
    }
};