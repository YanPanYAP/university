-- создание задачи
exec sp_CreateTask 
    @title = 'test task from procedure',
    @description = 'this task was created using sp_Createtask procedure',
    @creator_id = 1,
    @executor_id = 3,
    @priority = 2,
    @due_date = '2024-03-15';

-- смена статуса
exec sp_ChangeTaskStatus @task_id = 1, @new_status_id = 3, @changed_by = 2;

-- переназначение исполнителя
exec sp_ReassignExecutor @task_id = 2, @new_executor_id = 5;

-- удаление задачи
exec sp_DeleteTask @task_id = 25;

-- добавление комментария
exec sp_AddComment @task_id = 1, @user_id = 2, @text = 'testing comment from procedure';

-- повысить приоритет
exec sp_IncreasePriority @task_id = 3, @increment = 2;

-- создать напоминание
exec sp_SetTaskReminder @task_id = 4, @reminder_date = '2024-03-20 10:00:00';

-- массовое назначение задач
exec sp_AssignMultipleTasks @executor_id = 3, @task_ids = '8,3,1';

-- архивирование задачи
exec sp_ArchiveTask @task_id = 8;

-- отметить как важную
exec sp_MarkTaskImportant @task_id = 8;


-- количество задач пользователя
select dbo.fn_CountUserTasks(3) as usertaskscount;

-- количество просроченных задач
select dbo.fn_CountOverdueTasks() as overtaskscount;

-- средний приоритет
select dbo.fn_AvgPriority(3) as averagepriority;

-- дата последнего комментария
select dbo.fn_LastCommentDate(1) as lastcommentdate;

-- проверка просрочки
select dbo.fn_IsTaskOverdue(1) as isoverdue;

-- длительность задачи в днях
select dbo.fn_GetTaskDuration(1) as taskduration;

-- количество комментариев пользователя
select dbo.fn_CountUserComments(3) as usercommentscount;

-- список просроченных задач
select dbo.fn_GetOverdueTasksList() as overtaskslist;

-- количество задач с высоким приоритетом
select dbo.fn_GetHighPriorityTasks() as highprioritycount;

-- дата последнего действия исполнителя
select dbo.fn_LastExecutorAction(3) as lastactiondate;


select * from task order by task_id;
select * from comment order by comment_id;
select * from taskhistory order by history_id;