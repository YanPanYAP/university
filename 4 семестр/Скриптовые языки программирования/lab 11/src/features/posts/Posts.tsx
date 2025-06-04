import React, { useEffect } from "react";
import { useSelector, useDispatch } from "react-redux";
import type { RootState, AppDispatch } from "../../app/store";
import { fetchPostsThunk } from "./postsSlice";
import PostForm from "../../components/PostForm";
import PostItem from "../../components/PostItem";
import type { Post } from "./postsAPI";

const Posts: React.FC = () => {
  const dispatch = useDispatch<AppDispatch>();
  const { posts, status, error } = useSelector(
    (state: RootState) => state.posts
  );

  useEffect(() => {
    if (status === "idle") {
      dispatch(fetchPostsThunk());
    }
  }, [status, dispatch]);

  return (
    <div>
      <h1>Post Manager</h1>
      <PostForm />
      {status === "loading" && <p data-loading>Загрузка...</p>}
      {status === "failed" && <p data-error>Ошибка: {error}</p>}
      {status === "succeeded" && (
        <ul>
          {posts.map((post: Post) => (
            <PostItem key={post.id} post={post} />
          ))}
        </ul>
      )}
    </div>
  );
};

export default Posts;
