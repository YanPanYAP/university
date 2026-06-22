use("TaskControlMongoDB");

print("\n8.1. distinct() статусы");
printjson(db.assignments.distinct("status"));

print("\n8.2. distinct() отделы");
printjson(db.executors.distinct("department.name", { isActive: true }));
