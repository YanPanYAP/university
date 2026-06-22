LOAD DATA
CHARACTERSET AL32UTF8
INFILE '/tmp/lab11/import11-oracle.txt'
APPEND
INTO TABLE Task
FIELDS TERMINATED BY ';'
TRAILING NULLCOLS
(
title        CHAR(255) "UPPER(:title)",
description  CHAR(4000),
creator_id   INTEGER EXTERNAL,
executor_id  INTEGER EXTERNAL,
status_id    INTEGER EXTERNAL,
priority     DECIMAL EXTERNAL,
start_date   DATE "YYYY-MM-DD",
due_date     DATE "YYYY-MM-DD",
created_at   DATE "YYYY-MM-DD HH24:MI:SS",
task_id      EXPRESSION "seq_task.NEXTVAL"
)
