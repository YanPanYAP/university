# ЛР 24, задание 01 (`24-01`)

Express-приложение для WebDAV-хранилища с маршрутами:

- `POST /md/:ttttt` - создать директорию (`408`, если уже есть)
- `POST /rd/:ttttt` - удалить директорию (`408`, если не существует)
- `POST /up/:tttt` - загрузить файл в ФХ (`multipart/form-data`, поле `file`; перезапись включена; `408` при ошибке записи)
- `POST /down/:tttt` - скачать файл (`404`, если нет)
- `POST /del/:tttt` - удалить файл (`404`, если нет)
- `POST /copy/:tttt/:pppp` - копировать файл (`404`, если исходный не найден; `408` при ошибке записи)
- `POST /move/:tttt/:pppp` - переместить файл (`404`, если исходный не найден; `408` при ошибке записи)

## Запуск

```bash
npm i
cp .env.example .env
npm start
```

## Пример вызовов

```bash
# Создать директорию
curl -i -X POST http://localhost:3000/md/testdir

# Удалить директорию
curl -i -X POST http://localhost:3000/rd/testdir

# Загрузить файл
curl -i -X POST -F "file=@./sample.txt" http://localhost:3000/up/sample.txt

# Скачать файл
curl -i -X POST http://localhost:3000/down/sample.txt -o downloaded.txt

# Копировать
curl -i -X POST http://localhost:3000/copy/sample.txt/sample-copy.txt

# Переместить
curl -i -X POST http://localhost:3000/move/sample-copy.txt/sample-moved.txt

# Удалить файл
curl -i -X POST http://localhost:3000/del/sample-moved.txt
```
