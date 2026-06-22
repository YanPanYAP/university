use("TaskControlMongoDB");

print("\n10.1. Пустой $match");
db.assignments.aggregate([
  { $match: {} },
  {
    $group: {
      _id: "$status",
      count: { $sum: 1 },
      avgPriority: { $avg: "$priority" },
      minDueDate: { $min: "$dueDate" }
    }
  },
  { $sort: { count: -1, _id: 1 } }
]);

print("\n10.2. Непустой $match");
db.assignments.aggregate([
  { $match: { isCritical: true } },
  {
    $group: {
      _id: { department: "$department", status: "$status" },
      count: { $sum: 1 },
      tasks: { $push: "$title" }
    }
  },
  { $sort: { "_id.department": 1, "_id.status": 1 } }
]);
