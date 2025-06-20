<template>
  <div class="login-container">
    <h2>Login</h2>
    <form @submit.prevent="onLogin">
      <div>
        <label>Username</label>
        <input v-model="username" type="text" required />
      </div>
      <div>
        <label>Password</label>
        <input v-model="password" type="password" required />
      </div>
      <button type="submit">Login</button>
      <p v-if="error" class="error">{{ error }}</p>
    </form>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'Login',
  data() {
    return {
      username: '',
      password: '',
      error: ''
    };
  },
  methods: {
    async onLogin() {
      this.error = '';
      try {
        const res = await axios.post('/api/login', {
          username: this.username,
          password: this.password
        });
        if (res.data.success) {
          this.$router.push('/dashboard');
        } else {
          this.error = res.data.error || 'Login failed.';
        }
      } catch (e) {
        this.error = 'Login failed.';
      }
    }
  }
};
</script>

<style scoped>
.login-container {
  max-width: 300px;
  margin: 60px auto;
  padding: 2em;
  border: 1px solid #ccc;
  border-radius: 8px;
}
.error {
  color: red;
  margin-top: 1em;
}
</style> 