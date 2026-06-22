use("TaskControlMongoDB");

print("\n7. limit() и skip()");
db.assignments.find(
  {},
  { _id: 0, taskId: 1, title: 1, status: 1 }
).sort({ taskId: 1 }).skip(3).limit(3);
