module.exports = [
    { method: 'GET',    path: '/users',      controller: 'UserController', action: 'getUsers' },
    { method: 'GET',    path: '/users/:id',  controller: 'UserController', action: 'getUserById' },
    { method: 'POST',   path: '/users',      controller: 'UserController', action: 'createUser' },
    { method: 'PUT',    path: '/users/:id',  controller: 'UserController', action: 'updateUser' },
    { method: 'DELETE', path: '/users/:id',  controller: 'UserController', action: 'deleteUser' },

    { method: 'GET',    path: '/products',      controller: 'ProductController', action: 'getProducts' },
    { method: 'GET',    path: '/products/:id',  controller: 'ProductController', action: 'getProductById' },
    { method: 'POST',   path: '/products',      controller: 'ProductController', action: 'createProduct' },
    { method: 'PUT',    path: '/products/:id',  controller: 'ProductController', action: 'updateProduct' },
    { method: 'DELETE', path: '/products/:id',  controller: 'ProductController', action: 'deleteProduct' }
];