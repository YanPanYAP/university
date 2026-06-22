use("TaskControlMongoDB");

print("\n9.1. aggregate() по статусам");
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

print("\n9.2. aggregate() теги");
db.assignments.aggregate([
  { $match: {} },
  { $unwind: "$tags" },
  { $group: { _id: "$tags", count: { $sum: 1 } } },
  { $match: { count: { $gte: 2 } } },
  { $sort: { count: -1, _id: 1 } }
]);

print("\n9.3. aggregate() $lookup");
db.assignments.aggregate([
  { $match: { priority: { $lte: 2 } } },
  {
    $lookup: {
      from: "executors",
      localField: "executorId",
      foreignField: "userId",
      as: "executor"
    }
  },
  { $unwind: "$executor" },
  {
    $project: {
      _id: 0,
      taskId: 1,
      title: 1,
      status: 1,
      priority: 1,
      executorName: "$executor.fullName",
      executorRole: "$executor.role",
      executorDepartment: "$executor.department.name"
    }
  },
  { $sort: { priority: 1, taskId: 1 } }
]);
