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

/* 1. Smooth Transitions for All Elements */
* {
  transition: background 0.2s, color 0.2s, border 0.2s, box-shadow 0.2s, transform 0.1s;
}

/* 2. Subtle Card Hover Effect */
.card, .summary-card {
  transition: box-shadow 0.2s, transform 0.15s;
}
.card:hover, .summary-card:hover {
  box-shadow: 0 8px 32px rgba(60,60,60,0.18);
  transform: translateY(-4px) scale(1.03);
}

/* 3. Glassmorphism for Cards/Sections */
.glass {
  background: rgba(255,255,255,0.7);
  box-shadow: 0 4px 32px rgba(60,60,60,0.10);
  border-radius: 16px;
  backdrop-filter: blur(8px);
  border: 1px solid rgba(255,255,255,0.3);
}

/* 4. Animated Gradient Text for Headings */
.gradient-text {
  background: linear-gradient(90deg, #5B2A9D 0%, #00BFAE 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  animation: gradient-move 3s linear infinite alternate;
}
@keyframes gradient-move {
  0% { background-position: 0% 50%; }
  100% { background-position: 100% 50%; }
}

/* 5. Floating Action Button (FAB) */
.fab {
  position: fixed;
  bottom: 2.5em;
  right: 2.5em;
  background: var(--accent, #00BFAE);
  color: #fff;
  border-radius: 50%;
  width: 64px;
  height: 64px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 2em;
  box-shadow: 0 4px 24px rgba(60,60,60,0.18);
  cursor: pointer;
  z-index: 100;
  border: none;
}
.fab:hover {
  background: var(--primary, #5B2A9D);
}

/* 6. Subtle Section Divider */
.divider {
  width: 100%;
  height: 2px;
  background: linear-gradient(90deg, #5B2A9D 0%, #00BFAE 100%);
  opacity: 0.15;
  margin: 2em 0;
  border: none;
  border-radius: 1px;
}

/* Improved Dropdown Menus */
select {
  appearance: none;
  background: #fff;
  color: #222;
  border: 2px solid var(--primary, #5B2A9D);
  border-radius: 8px;
  padding: 0.7em 2.5em 0.7em 1em;
  font-size: 1.1em;
  font-family: inherit;
  box-shadow: 0 2px 8px rgba(60,60,60,0.07);
  transition: border 0.2s, box-shadow 0.2s;
  outline: none;
  margin: 0.5em 0;
  cursor: pointer;
  background-image: url('data:image/svg+xml;utf8,<svg fill="%235B2A9D" height="20" viewBox="0 0 24 24" width="20" xmlns="http://www.w3.org/2000/svg"><path d="M7 10l5 5 5-5z"/></svg>');
  background-repeat: no-repeat;
  background-position: right 1em center;
  background-size: 1.2em;
}
select:focus, select:hover {
  border-color: var(--accent, #00BFAE);
  box-shadow: 0 4px 16px rgba(0,191,174,0.10);
}

/* Improved Text Fields */
input[type="text"], input[type="password"], input[type="email"], input[type="number"], textarea {
  background: #fff;
  color: #222;
  border: 2px solid var(--primary, #5B2A9D);
  border-radius: 8px;
  padding: 0.7em 1em;
  font-size: 1.1em;
  font-family: inherit;
  box-shadow: 0 2px 8px rgba(60,60,60,0.07);
  transition: border 0.2s, box-shadow 0.2s;
  outline: none;
  margin: 0.5em 0;
  width: 100%;
  max-width: 400px;
  display: block;
}
input[type="text"]:focus, input[type="password"]:focus, input[type="email"]:focus, input[type="number"]:focus, textarea:focus {
  border-color: var(--accent, #00BFAE);
  box-shadow: 0 4px 16px rgba(0,191,174,0.10);
}
input[type="text"]:hover, input[type="password"]:hover, input[type="email"]:hover, input[type="number"]:hover, textarea:hover {
  border-color: var(--primary-light, #7C4DFF);
}

/* Placeholder styling */
input::placeholder, textarea::placeholder {
  color: #888;
  opacity: 1;
  font-size: 1em;
}
</style> 