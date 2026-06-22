use("TaskControlMongoDB");

db.assignments.drop();
db.executors.drop();

db.executors.insertMany([
  {
    userId: 1,
    fullName: "John Smith",
    email: "john.smith@company.com",
    role: "Administrator",
    department: { id: 1, name: "Head Office", parentId: null },
    skills: ["management", "security", "reporting"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2020-04-10T00:00:00Z")
  },
  {
    userId: 2,
    fullName: "Sarah Johnson",
    email: "sarah.johnson@company.com",
    role: "Manager",
    department: { id: 2, name: "Development Department", parentId: 1 },
    skills: ["planning", "backend", "review"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2021-02-17T00:00:00Z")
  },
  {
    userId: 3,
    fullName: "Michael Brown",
    email: "michael.brown@company.com",
    role: "Executor",
    department: { id: 5, name: "Mobile Development", parentId: 2 },
    skills: ["auth", "api", "mobile"],
    isActive: true,
    workload: 3,
    hiredAt: ISODate("2022-05-21T00:00:00Z")
  },
  {
    userId: 4,
    fullName: "Emily Davis",
    email: "emily.davis@company.com",
    role: "Executor",
    department: { id: 6, name: "Web Development", parentId: 2 },
    skills: ["database", "frontend", "validation"],
    isActive: true,
    workload: 2,
    hiredAt: ISODate("2022-08-12T00:00:00Z")
  },
  {
    userId: 5,
    fullName: "Robert Wilson",
    email: "robert.wilson@company.com",
    role: "Manager",
    department: { id: 3, name: "Testing Department", parentId: 1 },
    skills: ["testing", "quality", "review"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2021-11-03T00:00:00Z")
  },
  {
    userId: 6,
    fullName: "Jennifer Lee",
    email: "jennifer.lee@company.com",
    role: "Executor",
    department: { id: 7, name: "Manual Testing", parentId: 3 },
    skills: ["manual testing", "api testing"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2023-01-19T00:00:00Z")
  },
  {
    userId: 7,
    fullName: "David Miller",
    email: "david.miller@company.com",
    role: "Executor",
    department: { id: 8, name: "Automation Testing", parentId: 3 },
    skills: ["automation", "performance", "sql"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2020-12-01T00:00:00Z")
  },
  {
    userId: 8,
    fullName: "Lisa Anderson",
    email: "lisa.anderson@company.com",
    role: "Manager",
    department: { id: 4, name: "Analytics Department", parentId: 1 },
    skills: ["analytics", "documentation", "coordination"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2020-09-14T00:00:00Z")
  },
  {
    userId: 9,
    fullName: "James Taylor",
    email: "james.taylor@company.com",
    role: "Executor",
    department: { id: 9, name: "Business Analytics", parentId: 4 },
    skills: ["ui", "dashboards", "analytics"],
    isActive: false,
    workload: 1,
    hiredAt: ISODate("2023-04-05T00:00:00Z")
  },
  {
    userId: 10,
    fullName: "Maria Garcia",
    email: "maria.garcia@company.com",
    role: "Executor",
    department: { id: 10, name: "System Analytics", parentId: 4 },
    skills: ["monitoring", "systems", "documentation"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2021-07-30T00:00:00Z")
  },
  {
    userId: 11,
    fullName: "William Jones",
    email: "william.jones@company.com",
    role: "Administrator",
    department: { id: 1, name: "Head Office", parentId: null },
    skills: ["payments", "security", "release"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2019-03-22T00:00:00Z")
  },
  {
    userId: 12,
    fullName: "Elizabeth Martin",
    email: "elizabeth.martin@company.com",
    role: "Manager",
    department: { id: 11, name: "DevOps Team", parentId: 2 },
    skills: ["devops", "monitoring", "email"],
    isActive: true,
    workload: 1,
    hiredAt: ISODate("2022-10-09T00:00:00Z")
  }
]);

db.assignments.insertMany([
  {
    taskId: 1,
    title: "Implement user authentication",
    description: "Create login/register functionality with JWT tokens",
    creatorId: 1,
    executorId: 3,
    executorName: "Michael Brown",
    department: "Mobile Development",
    status: "In Progress",
    priority: 1,
    tags: ["backend", "security", "jwt"],
    startDate: ISODate("2024-01-15T00:00:00Z"),
    dueDate: ISODate("2024-01-30T00:00:00Z"),
    createdAt: ISODate("2024-01-15T09:00:00Z"),
    comments: [
      { userId: 3, text: "Started working on authentication endpoints", createdAt: ISODate("2024-01-15T10:30:00Z") },
      { userId: 1, text: "Please make sure to add refresh tokens", createdAt: ISODate("2024-01-15T14:20:00Z") }
    ],
    attachments: [{ fileName: "auth_spec.docx", filePath: "/attachments/auth_spec.docx" }],
    history: [
      { oldStatus: "New", newStatus: "In Progress", changedBy: 1, changedAt: ISODate("2024-01-15T10:35:00Z") },
      { oldStatus: "In Progress", newStatus: "New", changedBy: 3, changedAt: ISODate("2024-01-16T09:20:00Z") }
    ]
  },
  {
    taskId: 2,
    title: "Design database schema",
    description: "Create ERD and implement tables for the new project",
    creatorId: 2,
    executorId: 4,
    executorName: "Emily Davis",
    department: "Web Development",
    status: "Review",
    priority: 2,
    tags: ["database", "erd", "sql"],
    startDate: ISODate("2024-01-16T00:00:00Z"),
    dueDate: ISODate("2024-01-25T00:00:00Z"),
    createdAt: ISODate("2024-01-16T10:30:00Z"),
    comments: [
      { userId: 4, text: "Created initial ERD, need review", createdAt: ISODate("2024-01-17T11:45:00Z") },
      { userId: 2, text: "Looks good, add indexing strategy", createdAt: ISODate("2024-01-17T13:15:00Z") }
    ],
    attachments: [{ fileName: "database_erd.png", filePath: "/attachments/database_erd.png" }],
    history: [{ oldStatus: "New", newStatus: "Review", changedBy: 2, changedAt: ISODate("2024-01-18T14:30:00Z") }]
  },
  {
    taskId: 3,
    title: "Create responsive header",
    description: "Implement mobile-friendly navigation bar",
    creatorId: 3,
    executorId: 5,
    executorName: "Robert Wilson",
    department: "Testing Department",
    status: "New",
    priority: 3,
    tags: ["frontend", "ui", "responsive"],
    startDate: ISODate("2024-01-17T00:00:00Z"),
    dueDate: ISODate("2024-01-28T00:00:00Z"),
    createdAt: ISODate("2024-01-17T11:15:00Z"),
    comments: [
      { userId: 5, text: "Header is responsive on all devices", createdAt: ISODate("2024-01-20T16:30:00Z") },
      { userId: 3, text: "Great work, approved", createdAt: ISODate("2024-01-21T09:45:00Z") }
    ],
    attachments: [{ fileName: "header_mockup.fig", filePath: "/attachments/header_mockup.fig" }],
    history: [{ oldStatus: "New", newStatus: "New", changedBy: 3, changedAt: ISODate("2024-01-19T08:15:00Z") }]
  },
  {
    taskId: 4,
    title: "Write unit tests for API",
    description: "Cover all endpoints with unit tests",
    creatorId: 4,
    executorId: 6,
    executorName: "Jennifer Lee",
    department: "Manual Testing",
    status: "In Progress",
    priority: 1,
    tags: ["testing", "api", "unit-tests"],
    startDate: ISODate("2024-01-18T00:00:00Z"),
    dueDate: ISODate("2024-01-22T00:00:00Z"),
    createdAt: ISODate("2024-01-18T14:20:00Z"),
    comments: [
      { userId: 6, text: "Unit tests completed for auth endpoints", createdAt: ISODate("2024-01-19T14:20:00Z") },
      { userId: 4, text: "Add more edge cases", createdAt: ISODate("2024-01-19T15:40:00Z") }
    ],
    attachments: [{ fileName: "unit_tests.zip", filePath: "/attachments/unit_tests.zip" }],
    history: [
      { oldStatus: "New", newStatus: "In Progress", changedBy: 4, changedAt: ISODate("2024-01-20T10:50:00Z") },
      { oldStatus: "In Progress", newStatus: "Review", changedBy: 6, changedAt: ISODate("2024-01-21T13:25:00Z") }
    ],
    reminderAt: ISODate("2024-03-20T10:00:00Z")
  },
  {
    taskId: 5,
    title: "Optimize SQL queries",
    description: "Improve performance of slow database queries",
    creatorId: 5,
    executorId: 7,
    executorName: "David Miller",
    department: "Automation Testing",
    status: "Completed",
    priority: 2,
    tags: ["database", "performance", "sql"],
    startDate: ISODate("2024-01-19T00:00:00Z"),
    dueDate: ISODate("2024-02-05T00:00:00Z"),
    createdAt: ISODate("2024-01-19T08:45:00Z"),
    comments: [
      { userId: 7, text: "Optimized main queries, 50% faster", createdAt: ISODate("2024-01-25T12:10:00Z") },
      { userId: 5, text: "Excellent performance improvement", createdAt: ISODate("2024-01-26T10:30:00Z") }
    ],
    attachments: [{ fileName: "query_optimization.sql", filePath: "/attachments/query_optimization.sql" }],
    history: [{ oldStatus: "New", newStatus: "Completed", changedBy: 5, changedAt: ISODate("2024-01-22T15:40:00Z") }]
  },
  {
    taskId: 6,
    title: "Setup CI/CD pipeline",
    description: "Configure Jenkins for automated deployment",
    creatorId: 6,
    executorId: 8,
    executorName: "Lisa Anderson",
    department: "Analytics Department",
    status: "On Hold",
    priority: 1,
    tags: ["devops", "jenkins", "deployment"],
    startDate: ISODate("2024-01-20T00:00:00Z"),
    dueDate: ISODate("2024-01-27T00:00:00Z"),
    createdAt: ISODate("2024-01-20T13:10:00Z"),
    comments: [
      { userId: 8, text: "Jenkins pipeline configured", createdAt: ISODate("2024-01-24T15:55:00Z") },
      { userId: 6, text: "Tested deployment, works fine", createdAt: ISODate("2024-01-25T09:20:00Z") }
    ],
    attachments: [{ fileName: "jenkins_config.xml", filePath: "/attachments/jenkins_config.xml" }],
    history: [
      { oldStatus: "New", newStatus: "On Hold", changedBy: 6, changedAt: ISODate("2024-01-23T09:55:00Z") },
      { oldStatus: "On Hold", newStatus: "New", changedBy: 8, changedAt: ISODate("2024-01-24T11:10:00Z") }
    ]
  },
  {
    taskId: 7,
    title: "Create user dashboard",
    description: "Design and implement user profile page",
    creatorId: 7,
    executorId: 9,
    executorName: "James Taylor",
    department: "Business Analytics",
    status: "Cancelled",
    priority: 3,
    tags: ["frontend", "dashboard", "ui"],
    startDate: ISODate("2024-01-21T00:00:00Z"),
    dueDate: ISODate("2024-01-26T00:00:00Z"),
    createdAt: ISODate("2024-01-21T15:30:00Z"),
    comments: [
      { userId: 9, text: "Dashboard UI ready for review", createdAt: ISODate("2024-01-28T13:40:00Z") },
      { userId: 7, text: "Minor adjustments needed", createdAt: ISODate("2024-01-28T15:15:00Z") }
    ],
    attachments: [{ fileName: "dashboard_design.fig", filePath: "/attachments/dashboard_design.fig" }],
    history: [{ oldStatus: "New", newStatus: "Cancelled", changedBy: 7, changedAt: ISODate("2024-01-25T14:35:00Z") }]
  },
  {
    taskId: 8,
    title: "Document REST API",
    description: "Write API documentation using Swagger",
    creatorId: 8,
    executorId: 10,
    executorName: "Maria Garcia",
    department: "System Analytics",
    status: "Need Clarification",
    priority: 2,
    tags: ["documentation", "api", "swagger"],
    startDate: ISODate("2024-01-22T00:00:00Z"),
    dueDate: ISODate("2024-02-10T00:00:00Z"),
    createdAt: ISODate("2024-01-22T09:55:00Z"),
    comments: [
      { userId: 10, text: "API documentation completed", createdAt: ISODate("2024-01-29T11:30:00Z") },
      { userId: 8, text: "Looks comprehensive, good job", createdAt: ISODate("2024-01-30T10:45:00Z") }
    ],
    attachments: [{ fileName: "swagger_docs.json", filePath: "/attachments/swagger_docs.json" }],
    history: [{ oldStatus: "New", newStatus: "Need Clarification", changedBy: 8, changedAt: ISODate("2024-01-26T16:50:00Z") }]
  },
  {
    taskId: 9,
    title: "Fix payment gateway bug",
    description: "Resolve issues with payment processing",
    creatorId: 9,
    executorId: 11,
    executorName: "William Jones",
    department: "Head Office",
    status: "In Progress",
    priority: 1,
    tags: ["payments", "bugfix", "production"],
    startDate: ISODate("2024-01-23T00:00:00Z"),
    dueDate: ISODate("2024-01-24T00:00:00Z"),
    createdAt: ISODate("2024-01-23T16:40:00Z"),
    comments: [
      { userId: 11, text: "Fixed payment gateway timeout issue", createdAt: ISODate("2024-01-23T17:20:00Z") },
      { userId: 9, text: "Verified fix, ready for production", createdAt: ISODate("2024-01-24T09:15:00Z") }
    ],
    attachments: [{ fileName: "payment_fix.patch", filePath: "/attachments/payment_fix.patch" }],
    history: [
      { oldStatus: "New", newStatus: "In Progress", changedBy: 9, changedAt: ISODate("2024-01-27T10:05:00Z") },
      { oldStatus: "In Progress", newStatus: "Completed", changedBy: 11, changedAt: ISODate("2024-01-28T12:20:00Z") }
    ],
    externalTicket: "PAY-204"
  },
  {
    taskId: 10,
    title: "Setup monitoring system",
    description: "Install and configure Prometheus and Grafana",
    creatorId: 10,
    executorId: 12,
    executorName: "Elizabeth Martin",
    department: "DevOps Team",
    status: "Review",
    priority: 3,
    tags: ["monitoring", "grafana", "devops"],
    startDate: ISODate("2024-01-24T00:00:00Z"),
    dueDate: ISODate("2024-02-15T00:00:00Z"),
    createdAt: ISODate("2024-01-24T11:25:00Z"),
    comments: [
      { userId: 12, text: "Grafana dashboards created", createdAt: ISODate("2024-01-30T14:50:00Z") },
      { userId: 10, text: "Add CPU usage metrics", createdAt: ISODate("2024-01-31T11:25:00Z") }
    ],
    attachments: [{ fileName: "grafana_config.json", filePath: "/attachments/grafana_config.json" }],
    history: [{ oldStatus: "New", newStatus: "Review", changedBy: 10, changedAt: ISODate("2024-01-29T15:45:00Z") }]
  },
  {
    taskId: 11,
    title: "Implement search functionality",
    description: "Add full-text search to the application",
    creatorId: 11,
    executorId: 3,
    executorName: "Michael Brown",
    department: "Mobile Development",
    status: "Ready for Testing",
    priority: 2,
    tags: ["search", "backend", "filters"],
    startDate: ISODate("2024-02-01T00:00:00Z"),
    dueDate: ISODate("2024-02-20T00:00:00Z"),
    createdAt: ISODate("2024-01-25T10:00:00Z"),
    comments: [
      { userId: 13, text: "Search implementation in progress", createdAt: ISODate("2024-02-05T10:10:00Z") },
      { userId: 11, text: "Consider adding filters", createdAt: ISODate("2024-02-05T14:30:00Z") }
    ],
    attachments: [{ fileName: "search_algorithm.pdf", filePath: "/attachments/search_algorithm.pdf" }],
    history: [{ oldStatus: "New", newStatus: "Ready for Testing", changedBy: 11, changedAt: ISODate("2024-02-02T09:30:00Z") }]
  },
  {
    taskId: 12,
    title: "Add email notifications",
    description: "Implement email alerts for task updates",
    creatorId: 12,
    executorId: 4,
    executorName: "Emily Davis",
    department: "Web Development",
    status: "Testing",
    priority: 1,
    tags: ["email", "notifications", "backend"],
    startDate: ISODate("2024-02-02T00:00:00Z"),
    dueDate: ISODate("2024-02-18T00:00:00Z"),
    createdAt: ISODate("2024-01-26T14:15:00Z"),
    comments: [
      { userId: 14, text: "Email templates created", createdAt: ISODate("2024-02-06T13:20:00Z") },
      { userId: 12, text: "Tested notifications, all good", createdAt: ISODate("2024-02-07T09:55:00Z") }
    ],
    attachments: [],
    history: [{ oldStatus: "New", newStatus: "Testing", changedBy: 12, changedAt: ISODate("2024-02-03T13:15:00Z") }]
  }
]);

db.assignments.updateOne(
  { taskId: 1 },
  {
    $set: { status: "Review", reviewedAt: ISODate("2024-01-29T12:00:00Z") },
    $push: { tags: "review" }
  }
);

db.assignments.updateMany(
  { priority: 1 },
  { $set: { isCritical: true } }
);

db.executors.updateOne(
  { userId: 3 },
  {
    $inc: { workload: 1 },
    $addToSet: { skills: "jwt" }
  }
);

db.assignments.updateMany({}, [{ $set: { priority: { $toInt: "$priority" } } }]);
db.executors.updateMany({}, [{ $set: { workload: { $toInt: "$workload" } } }]);

db.assignments.createIndex({ taskId: 1 }, { unique: true });
db.assignments.createIndex({ status: 1, priority: 1 });
db.executors.createIndex({ userId: 1 }, { unique: true });

print("Database TaskControlMongoDB is ready.");
print("Assignments: " + db.assignments.countDocuments());
print("Executors: " + db.executors.countDocuments());
