use("TaskControlMongoDB");

print("\n4.1. Условия");
db.assignments.find(
  { priority: { $lte: 2 }, status: { $in: ["Review", "In Progress"] } },
  { _id: 0, taskId: 1, title: 1, status: 1, priority: 1 }
);

print("\n4.2. $all");
db.assignments.find(
  { tags: { $all: ["backend", "review"] } },
  { _id: 0, taskId: 1, title: 1, tags: 1 }
);

print("\n4.3. $elemMatch");
db.assignments.find(
  { comments: { $elemMatch: { userId: 1 } } },
  { _id: 0, taskId: 1, title: 1, comments: 1 }
);

print("\n4.4. $exists");
db.assignments.find(
  { externalTicket: { $exists: true } },
  { _id: 0, taskId: 1, title: 1, externalTicket: 1 }
);

print("\n4.5. $type");
db.assignments.find(
  { priority: { $type: "int" } },
  { _id: 0, taskId: 1, title: 1, priority: 1 }
);

print("\n4.6. $regex");
db.executors.find(
  { email: { $regex: /^m.*@company\.com$/i } },
  { _id: 0, userId: 1, fullName: 1, email: 1 }
);
