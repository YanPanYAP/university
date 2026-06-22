use("TaskControlMongoDB");

print("\n5. Проекция");
db.assignments.find(
  {},
  { _id: 0, title: 1, status: 1, executorName: 1, dueDate: 1 }
);
