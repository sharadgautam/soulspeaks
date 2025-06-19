<template>
  <router-view @login="handleLogin" @logout="handleLogout" />
</template>

<script>
import router from './router';

export default {
  name: 'App',
  router,
  methods: {
    handleLogin(credentials) {
      // Simulate login (replace with real API call)
      if (credentials.username === 'admin' && credentials.password === 'admin') {
        localStorage.setItem('auth', 'true');
        this.$router.push('/dashboard');
      } else {
        // Find the Login component and set error
        const loginView = this.$children.find(c => c.$options.name === 'Login');
        if (loginView) loginView.error = 'Invalid credentials';
      }
    },
    handleLogout() {
      localStorage.removeItem('auth');
      this.$router.push('/login');
    }
  }
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  text-align: center;
  margin-top: 60px;
}
</style> 