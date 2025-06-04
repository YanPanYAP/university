import { createSlice, createAsyncThunk } from "@reduxjs/toolkit";
import axios from "axios";
import {
  type Post,
  type NewPost,
  fetchPosts,
  createPost,
  updatePost,
  deletePost,
} from "./postsAPI";

let localIdCounter = -1;

interface PostsState {
  posts: Post[];
  status: "idle" | "loading" | "succeeded" | "failed";
  error: string | null;
}

const initialState: PostsState = {
  posts: [],
  status: "idle",
  error: null,
};

export const fetchPostsThunk = createAsyncThunk(
  "posts/fetchPosts",
  async () => {
    return await fetchPosts();
  }
);

export const createPostThunk = createAsyncThunk(
  "posts/createPost",
  async (post: NewPost) => {
    const response = await createPost(post);
    return { ...response, id: localIdCounter-- };
  }
);

export const updatePostThunk = createAsyncThunk(
  "posts/updatePost",
  async (post: Post) => {
    console.log("updatePostThunk: Sending post:", post);
    try {
      if (post.id < 0) {
        const { title, body, userId } = post;
        const postData = { title, body, userId };
        console.log("updatePostThunk: Sending API request with:", postData);
        const response = await axios.put(
          "https://jsonplaceholder.typicode.com/posts/101",
          postData
        );
        console.log("updatePostThunk: API response:", response.data);
        const result = {
          id: post.id,
          title: post.title,
          body: post.body,
          userId: post.userId,
        };
        console.log("updatePostThunk: Returning:", result);
        return result;
      }
      const response = await updatePost(post);
      console.log("updatePostThunk: API response for existing post:", response);
      return response;
    } catch (error) {
      console.error("updatePostThunk: API error:", error);
      if (post.id < 0) {
        const result = {
          id: post.id,
          title: post.title,
          body: post.body,
          userId: post.userId,
        };
        console.log("updatePostThunk: API failed, returning:", result);
        return result;
      }
      throw error;
    }
  }
);

export const deletePostThunk = createAsyncThunk(
  "posts/deletePost",
  async (id: number) => {
    try {
      if (id < 0) {
        await axios.delete("https://jsonplaceholder.typicode.com/posts/101");
      } else {
        await deletePost(id);
      }
    } catch (error) {
      console.error("Ошибка при удалении поста:", error);
    }
    return id;
  }
);

const postsSlice = createSlice({
  name: "posts",
  initialState,
  reducers: {},
  extraReducers: (builder) => {
    builder
      .addCase(fetchPostsThunk.pending, (state) => {
        state.status = "loading";
      })
      .addCase(fetchPostsThunk.fulfilled, (state, action) => {
        state.status = "succeeded";
        state.posts = action.payload.slice().reverse();
      })
      .addCase(fetchPostsThunk.rejected, (state, action) => {
        state.status = "failed";
        state.error = action.error.message || "Ошибка при загрузке постов";
      })
      .addCase(createPostThunk.fulfilled, (state, action) => {
        state.posts.unshift(action.payload);
      })
      .addCase(updatePostThunk.fulfilled, (state, action) => {
        console.log(
          "updatePostThunk.fulfilled: Updating state with:",
          action.payload
        );
        const index = state.posts.findIndex(
          (post) => post.id === action.payload.id
        );
        console.log("updatePostThunk.fulfilled: Found index:", index);
        if (index !== -1) {
          state.posts[index] = action.payload;
          console.log(
            "updatePostThunk.fulfilled: Updated post:",
            state.posts[index]
          );
          console.log(
            "updatePostThunk.fulfilled: Current posts state:",
            state.posts
          );
        } else {
          console.warn(`Пост с id ${action.payload.id} не найден`);
        }
      })
      .addCase(updatePostThunk.rejected, (state, action) => {
        state.error = action.error.message || "Ошибка при обновлении поста";
        console.error("updatePostThunk.rejected:", state.error);
      })
      .addCase(deletePostThunk.fulfilled, (state, action) => {
        state.posts = state.posts.filter((post) => post.id !== action.payload);
      })
      .addCase(deletePostThunk.rejected, (state, action) => {
        state.posts = state.posts.filter((post) => post.id !== action.payload);
        state.error = action.error.message || "Ошибка при удалении поста";
      });
  },
});

export default postsSlice.reducer;
