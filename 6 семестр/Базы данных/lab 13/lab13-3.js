use("TaskControlMongoDB");

print("\n3.1. Изменить один");
db.assignments.updateOne(
  { taskId: 1 },
  {
    $set: {
      status: "In Progress",
      isCritical: true
    }
  }
);

print("\n3.2. Обновить несколько");
db.assignments.updateMany(
  { priority: { $lte: 2 } },
  {
    $set: { isCritical: true },
    $addToSet: { tags: "high-priority" }
  }
);

print("\n3.3. Изменить один");
db.executors.updateOne(
  { userId: 1 },
  {
    $set: {
      role: "Senior Developer",
      isActive: true
    }
  }
);

print("\n3.4. Обновить несколько");
db.executors.updateMany(
  { "department.name": "Development" },
  {
    $set: { isActive: true }
  }
);
