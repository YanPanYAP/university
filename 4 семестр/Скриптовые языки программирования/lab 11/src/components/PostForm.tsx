import React, { useState, useEffect } from "react";
import { useDispatch } from "react-redux";
import type { AppDispatch } from "../app/store";
import { createPostThunk, updatePostThunk } from "../features/posts/postsSlice";
import type { Post, NewPost } from "../features/posts/postsAPI";

interface PostFormProps {
  post?: Post;
  onSubmit?: () => void;
}

const PostForm: React.FC<PostFormProps> = ({ post, onSubmit }) => {
  const dispatch = useDispatch<AppDispatch>();
  const [title, setTitle] = useState(post?.title || "");
  const [body, setBody] = useState(post?.body || "");

  useEffect(() => {
    if (post) {
      console.log("PostForm: useEffect updating form with post:", post);
      setTitle(post.title);
      setBody(post.body);
    }
  }, [post]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!title.trim() || !body.trim()) {
      console.log("PostForm: Empty title or body, submission cancelled");
      return;
    }

    const newPost: NewPost = { title, body, userId: 1 };

    try {
      if (post) {
        const updatedPost = { ...post, title, body };
        console.log("PostForm: Updating post:", updatedPost);
        await dispatch(updatePostThunk(updatedPost)).unwrap();
      } else {
        console.log("PostForm: Creating post:", newPost);
        await dispatch(createPostThunk(newPost)).unwrap();
      }
      setTitle("");
      setBody("");
    } catch (error) {
      console.error("PostForm: Error submitting form:", error);
    } finally {
      console.log("PostForm: Form submission completed, calling onSubmit");
      if (onSubmit) onSubmit();
    }
  };

  return (
    <form onSubmit={handleSubmit}>
      <div>
        <label>Заголовок:</label>
        <input
          type="text"
          value={title}
          onChange={(e) => setTitle(e.target.value)}
          required
          placeholder="Введите заголовок"
        />
      </div>
      <div>
        <label>Содержание:</label>
        <textarea
          value={body}
          onChange={(e) => setBody(e.target.value)}
          required
          placeholder="Введите содержание"
        />
      </div>
      <button type="submit">{post ? "Обновить" : "Добавить"}</button>
    </form>
  );
};

export default PostForm;
