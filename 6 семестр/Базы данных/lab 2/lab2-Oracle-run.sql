-- создание задачи
begin
    pr_create_task(
        p_title => 'test task from oracle',
        p_description => 'this task was created using pr_create_task procedure',
        p_creator_id => 1,
        p_executor_id => 3,
        p_priority => 2,
        p_due_date => date '2024-03-15'
    );
end;
/

-- смена статуса
begin
    pr_change_status(
        p_task_id => 1,
        p_new_status => 3,
        p_changed_by => 2
    );
end;
/

-- переназначение исполнителя
begin
    pr_reassign_executor(
        p_task_id => 2,
        p_new_executor => 5
    );
end;
/

-- удаление задачи
begin
    pr_delete_task(p_task_id => 25);
end;
/

-- добавление комментария
begin
    pr_add_comment(
        p_task_id => 1,
        p_user_id => 2,
        p_text => 'testing comment from oracle procedure'
    );
end;
/

-- повысить приоритет задачи
begin
    pr_increase_priority(
        p_task_id => 3,
        p_increment => 2
    );
end;
/

-- создать напоминание
begin
    pr_set_task_reminder(
        p_task_id => 4,
        p_reminder_date => date '2024-03-20'
    );
end;
/

-- массовое назначение задач
begin
    pr_assign_multiple_tasks(
        p_executor_id => 7,
        p_task_list => sys.odcinumberlist(5, 6, 7)
    );
end;
/

-- архивирование завершенной задачи
begin
    pr_archive_task(p_task_id => 8);
end;
/

-- добавить отметку важности
begin
    pr_mark_task_important(p_task_id => 7);
end;
/

-- количество задач пользователя
select fn_count_user_tasks(3) as user_tasks_count from dual;

-- количество просроченных задач
select fn_count_overdue_tasks() as overdue_tasks_count from dual;

-- средний приоритет
select fn_avg_priority(3) as average_priority from dual;

-- дата последнего комментария
select fn_last_comment_date(1) as last_comment_date from dual;

-- проверка просрочки
select fn_is_task_overdue(1) as is_overdue from dual;

-- длительность задачи в днях
select fn_get_task_duration(1) as task_duration from dual;

-- количество комментариев пользователя
select fn_count_user_comments(3) as user_comments_count from dual;

-- список просроченных задач
select fn_get_overdue_tasks_list() as overdue_tasks_list from dual;

-- количество задач с высоким приоритетом
select fn_get_high_priority_tasks() as high_priority_count from dual;

-- дата последнего действия исполнителя
select fn_last_executor_action(3) as last_action_date from dual;

select * from task order by task_id;
select * from task_comment order by comment_id;
select * from taskhistory order by history_id;