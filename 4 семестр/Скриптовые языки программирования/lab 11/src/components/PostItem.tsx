import React, { useState } from "react";
import { useDispatch } from "react-redux";
import type { AppDispatch } from "../app/store";
import { deletePostThunk } from "../features/posts/postsSlice";
import PostForm from "./PostForm";
import type { Post } from "../features/posts/postsAPI";

interface PostItemProps {
  post: Post;
}

const PostItem: React.FC<PostItemProps> = ({ post }) => {
  const dispatch = useDispatch<AppDispatch>();
  const [isEditing, setIsEditing] = useState(false);

  const handleDelete = () => {
    console.log("PostItem: Deleting post with id:", post.id);
    dispatch(deletePostThunk(post.id));
  };

  const handleSubmit = () => {
    console.log("PostItem: Edit form submitted for post:", post.id);
    setIsEditing(false);
  };

  return (
    <li>
      {isEditing ? (
        <PostForm post={post} onSubmit={handleSubmit} />
      ) : (
        <div>
          <h3>{post.title}</h3>
          <p>{post.body}</p>
          <button onClick={() => setIsEditing(true)}>Редактировать</button>
          <button className="delete" onClick={handleDelete}>
            Удалить
          </button>
        </div>
      )}
    </li>
  );
};

export default PostItem;
