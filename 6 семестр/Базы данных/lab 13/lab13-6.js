use("TaskControlMongoDB");

print("\n6.1. count() все");
print(db.assignments.count());

print("\n6.2. count() с фильтром");
print(db.assignments.count({ isCritical: true }));
