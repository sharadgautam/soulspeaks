<template>
  <div class="layout-bg">
    <div class="bg-visuals">
      <svg class="bg-shape shape1" viewBox="0 0 400 400"><circle cx="200" cy="200" r="200" fill="url(#grad1)" fill-opacity="0.18"/></svg>
      <svg class="bg-shape shape2" viewBox="0 0 400 400"><rect x="0" y="0" width="400" height="400" rx="120" fill="url(#grad2)" fill-opacity="0.13"/></svg>
      <svg width="0" height="0">
        <defs>
          <linearGradient id="grad1" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#00BFAE"/>
            <stop offset="100%" stop-color="#7C4DFF"/>
          </linearGradient>
          <linearGradient id="grad2" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stop-color="#5B2A9D"/>
            <stop offset="100%" stop-color="#00BFAE"/>
          </linearGradient>
        </defs>
      </svg>
    </div>
    <header class="navbar">
      <div class="logo">
        <img src="/logo.png" alt="SoulSpeaks Logo" />
        <span class="brand">SoulSpeaks</span>
      </div>
      <nav class="nav-links">
        <div class="nav-item">
          <router-link to="/dashboard" aria-label="Dashboard">🏠</router-link>
          <span class="nav-tooltip">Dashboard</span>
        </div>
        <div class="nav-item">
          <router-link to="/buy-dids" aria-label="Buy DIDs">📞</router-link>
          <span class="nav-tooltip">Buy DIDs</span>
        </div>
        <div class="nav-item">
          <router-link to="/add-trunk" aria-label="Add Trunk">🔗</router-link>
          <span class="nav-tooltip">Add Trunk</span>
        </div>
        <div class="nav-item">
          <router-link to="/manage-dids" aria-label="Manage DIDs">🗂️</router-link>
          <span class="nav-tooltip">Manage DIDs</span>
        </div>
        <div class="nav-item">
          <router-link to="/add-funds" aria-label="Add Funds">💰</router-link>
          <span class="nav-tooltip">Add Funds</span>
        </div>
        <div class="nav-item">
          <router-link to="/transactions" aria-label="Transactions">📊</router-link>
          <span class="nav-tooltip">Transactions</span>
        </div>
        <div class="nav-item">
          <a href="/sip-client" target="_blank" rel="noopener" aria-label="Cloud PBX">☁️</a>
          <span class="nav-tooltip">Cloud PBX</span>
        </div>
      </nav>
      <div v-if="showProfileAndLogout" class="navbar-append">
        <div class="profile-container">
          <span class="profile-icon" tabindex="0">👤
            <span class="profile-tooltip">{{ username }}</span>
          </span>
        </div>
        <button class="logout-btn" @click="logout">Logout</button>
      </div>
    </header>
    <main class="main-content">
      <slot />
    </main>
    <footer class="footer">
      <span>© {{ new Date().getFullYear() }} SoulSpeaks. All rights reserved.</span>
    </footer>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'PageLayout',
  data() {
    return {
      username: 'User'
    };
  },
  computed: {
    showProfileAndLogout() {
      // Hide on login and signup pages
      const path = this.$route && this.$route.path;
      return path !== '/login' && path !== '/signup';
    }
  },
  methods: {
    logout() {
      localStorage.removeItem('auth');
      localStorage.removeItem('jwt');
      this.$router.push('/login');
    },
    async fetchUsername() {
      const token = localStorage.getItem('jwt');
      if (!token) return;
      try {
        const res = await axios.get('/api/me', {
          headers: { Authorization: `Bearer ${token}` }
        });
        this.username = res.data.username || 'User';
      } catch (e) {
        this.username = 'User';
      }
    }
  },
  mounted() {
    this.fetchUsername();
  },
  watch: {
    // Refetch username if route changes (e.g., after login)
    $route() {
      this.fetchUsername();
    }
  }
};
</script>

<style scoped>
/* Remove navbar, logo, nav-links, nav-item, nav-tooltip, .brand, .main-content, .footer, :root from here and move to global style below */
.layout-bg {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background: var(--bg);
  position: relative;
  overflow-x: hidden;
}
.bg-visuals {
  position: fixed;
  inset: 0;
  z-index: 0;
  pointer-events: none;
  width: 100vw;
  height: 100vh;
}
.bg-shape {
  position: absolute;
  width: 600px;
  height: 600px;
  filter: blur(8px);
  animation: float 16s ease-in-out infinite alternate;
}
.shape1 {
  top: -180px;
  left: -120px;
  animation-delay: 0s;
}
.shape2 {
  bottom: -160px;
  right: -140px;
  animation-delay: 4s;
}
@keyframes float {
  0% { transform: scale(1) translateY(0px); }
  100% { transform: scale(1.08) translateY(30px); }
}
</style>

<style>
:root {
  --primary: #5B2A9D; /* Deep blue-violet */
  --primary-light: #7C4DFF;
  --accent: #00BFAE; /* Vibrant teal */
  --bg: #F7F8FA; /* Soft off-white */
  --card-bg: #fff;
  --shadow: 0 4px 24px rgba(60,60,60,0.08);
  --radius: 16px;
  --footer-bg: #ece6f6;
}
.navbar {
  display: grid;
  grid-template-columns: auto 1fr auto;
  align-items: center;
  padding: 1.2em 2em;
  background: #fff;
  box-shadow: 0 2px 12px rgba(60,60,60,0.10);
  position: sticky;
  top: 0;
  z-index: 10;
}
.logo {
  position: static;
  left: unset;
  display: flex;
  align-items: center;
  gap: 0.7em;
  justify-self: start;
}
.logo img {
  width: 38px;
  height: 38px;
  border-radius: 8px;
  background: #fff;
  object-fit: contain;
  box-shadow: 0 2px 8px rgba(60,60,60,0.10);
}
.brand {
  font-size: 1.5em;
  font-weight: 900;
  color: var(--accent);
  letter-spacing: -1px;
}
.nav-links {
  display: flex;
  gap: 1.5em;
  justify-self: center;
  justify-content: center;
}
.nav-item {
  position: relative;
  display: flex;
  flex-direction: column;
  align-items: center;
}
.nav-tooltip {
  visibility: hidden;
  opacity: 0;
  background: rgba(40, 40, 60, 0.95);
  color: #fff;
  text-align: center;
  border-radius: 6px;
  padding: 0.3em 0.9em;
  position: absolute;
  bottom: -2.2em;
  left: 50%;
  transform: translateX(-50%);
  white-space: nowrap;
  font-size: 0.98em;
  font-weight: 600;
  pointer-events: none;
  transition: opacity 0.18s, visibility 0.18s;
  z-index: 100;
  box-shadow: 0 2px 8px rgba(60,60,60,0.13);
}
.nav-item:hover .nav-tooltip,
.nav-item:focus-within .nav-tooltip {
  visibility: visible;
  opacity: 1;
}
.main-content {
  flex: 1;
  padding: 2.5em 1em 1.5em 1em;
  max-width: 1100px;
  margin: 0 auto;
  width: 100%;
  z-index: 1;
}
.footer {
  background: var(--footer-bg);
  text-align: center;
  padding: 1.2em 0 1em 0;
  font-size: 1.1em;
  color: var(--primary);
  font-weight: 600;
  letter-spacing: 0.5px;
  box-shadow: 0 -2px 12px rgba(60,60,60,0.07);
  border-top-left-radius: 16px;
  border-top-right-radius: 16px;
  z-index: 1;
}
@media (max-width: 700px) {
  .navbar {
    display: flex;
    flex-direction: column;
    align-items: flex-start;
    gap: 0.7em;
    padding: 1em 0.5em;
  }
  .logo {
    position: static;
    margin-bottom: 0.5em;
    left: unset;
  }
  .nav-links {
    justify-content: center;
    width: 100%;
    justify-self: center;
  }
  .main-content {
    padding: 1.2em 0.2em 1em 0.2em;
    max-width: 98vw;
  }
  .footer {
    font-size: 1em;
    padding: 1em 0 0.7em 0;
  }
}
.navbar-append {
  display: flex;
  align-items: center;
  gap: 0.7em;
  margin-left: 1.5em;
  justify-self: end;
}
.profile-container {
  display: inline-block;
  position: relative;
  margin-right: 1.2em;
}
.profile-icon {
  font-size: 1.6em;
  cursor: pointer;
  position: relative;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  border-radius: 50%;
  background: #f0f4ff;
  padding: 0.18em 0.38em;
  transition: background 0.18s;
}
.profile-icon:hover, .profile-icon:focus {
  background: #e0e7ff;
}
.profile-tooltip {
  visibility: hidden;
  opacity: 0;
  background: rgba(40, 40, 60, 0.95);
  color: #fff;
  text-align: center;
  border-radius: 6px;
  padding: 0.3em 1em;
  position: absolute;
  left: 50%;
  bottom: -2.2em;
  transform: translateX(-50%);
  white-space: nowrap;
  font-size: 1em;
  font-weight: 600;
  pointer-events: none;
  transition: opacity 0.18s, visibility 0.18s;
  z-index: 100;
  box-shadow: 0 2px 8px rgba(60,60,60,0.13);
}
.profile-icon:hover .profile-tooltip,
.profile-icon:focus .profile-tooltip {
  visibility: visible;
  opacity: 1;
}
.logout-btn {
  margin-left: 0.5em;
  background: #fff;
  color: var(--primary);
  border: 2px solid var(--primary);
  border-radius: 8px;
  padding: 0.7em 2em;
  font-size: 1.1em;
  font-weight: 700;
  cursor: pointer;
  transition: background 0.2s, color 0.2s, border 0.2s;
}
.logout-btn:hover {
  background: var(--primary);
  color: #fff;
  border-color: var(--primary-light);
}
</style> 