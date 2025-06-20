<template>
  <PageLayout>
    <template #navbar-append>
      <div class="profile-container">
        <span class="profile-icon" tabindex="0">👤
          <span class="profile-tooltip">{{ username }}</span>
        </span>
      </div>
      <button class="logout-btn" @click="$emit('logout')">Logout</button>
    </template>
    <div class="dashboard-container">
      <h2 class="dashboard-title">Welcome to SoulSpeaks Cloud PBX!</h2>
      <div class="summary-section">
        <h3>Account Summary</h3>
        <div class="summary-cards">
          <div class="summary-card">
            <span class="summary-label">Balance</span>
            <span class="summary-value">{{ balanceDisplay }}</span>
          </div>
          <div class="summary-card">
            <span class="summary-label">DIDs Owned</span>
            <span class="summary-value">{{ ownedDids.length }}</span>
          </div>
          <div class="summary-card">
            <span class="summary-label">Trunks</span>
            <span class="summary-value">{{ trunks.length }}</span>
          </div>
          <div class="summary-card">
            <span class="summary-label">Assignments</span>
            <span class="summary-value">{{ assignments.length }}</span>
          </div>
        </div>
      </div>
      <div class="usage-graph-section glass">
        <h3 class="section-title">Minute Usage (Last 7 Days)</h3>
        <svg class="usage-graph interactive" viewBox="0 0 600 240" @mousemove="onGraphHover" @mouseleave="hoverIndex = null">
          <polyline
            :points="usagePoints"
            fill="none"
            stroke="#5B2A9D"
            stroke-width="4"
            stroke-linecap="round"
            stroke-linejoin="round"
          />
          <g v-for="(val, i) in minuteUsage" :key="i">
            <circle :cx="80 + i*70" :cy="200 - val*2" r="8" fill="#00BFAE"
              @mouseenter="hoverIndex = i" @mouseleave="hoverIndex = null" />
            <text :x="80 + i*70" y="225" text-anchor="middle" font-size="16" fill="#888">{{ usageLabels[i] }}</text>
            <text :x="80 + i*70" :y="190 - val*2" text-anchor="middle" font-size="15" fill="#5B2A9D">{{ val }}</text>
            <g v-if="hoverIndex === i">
              <rect :x="60 + i*70" :y="160 - val*2" width="40" height="28" rx="8" fill="#fff" stroke="#00BFAE" stroke-width="2" />
              <text :x="80 + i*70" :y="178 - val*2" text-anchor="middle" font-size="14" fill="#5B2A9D" font-weight="bold">{{ val }} min</text>
            </g>
          </g>
        </svg>
      </div>
      <p v-if="message" class="message">{{ message }}</p>
    </div>
  </PageLayout>
</template>

<script>
import axios from 'axios';
import PageLayout from '../components/PageLayout.vue';
export default {
  name: 'Dashboard',
  components: { PageLayout },
  data() {
    return {
      balance: 0,
      ownedDids: [],
      trunks: [],
      assignments: [],
      message: '',
      minuteUsage: [30, 45, 20, 60, 50, 80, 40], // Example data
      usageLabels: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
      hoverIndex: null,
      username: localStorage.getItem('username') || 'User'
    };
  },
  computed: {
    balanceDisplay() {
      return `$${this.balance.toFixed(2)}`;
    },
    usagePoints() {
      // Map usage to SVG points (larger graph)
      return this.minuteUsage.map((val, i) => `${80 + i*70},${200 - val*2}`).join(' ');
    }
  },
  methods: {
    onGraphHover(e) {
      // Optionally, could add more interactivity here
    }
  },
  async mounted() {
    // Fetch account summary (mocked for now)
    try {
      // Example: const res = await axios.get('/api/account/summary');
      // this.balance = res.data.balance;
      // this.ownedDids = res.data.ownedDids;
      // this.trunks = res.data.trunks;
      // this.assignments = res.data.assignments;
      // For now, use mock data:
      this.balance = 25.50;
      this.ownedDids = ['+1234567890', '+1987654321'];
      this.trunks = ['sip:trunk1@sip.example.com', 'sip:trunk2@sip.example.com'];
      this.assignments = [
        { did: '+1234567890', trunk: 'sip:trunk1@sip.example.com' },
        { did: '+1987654321', trunk: 'sip:trunk2@sip.example.com' }
      ];
    } catch (e) {
      this.message = 'Failed to load account summary.';
    }
  }
};
</script>

<style scoped>
:root {
  --primary: #5B2A9D;
  --primary-light: #7C4DFF;
  --accent: #00BFAE;
  --bg: #F7F8FA;
  --card-bg: #fff;
  --shadow: 0 4px 24px rgba(60,60,60,0.08);
  --radius: 16px;
}
.dashboard-container {
  max-width: 900px;
  margin: 0 auto;
  padding: 0;
  background: none;
  border-radius: 0;
  box-shadow: none;
  text-align: center;
  display: flex;
  flex-direction: column;
  gap: 2em;
}
.dashboard-title {
  font-size: 2.2em;
  font-weight: 800;
  letter-spacing: -1px;
  color: var(--primary);
  margin-bottom: 0.5em;
}
.summary-section {
  background: var(--card-bg);
  border-radius: var(--radius);
  box-shadow: var(--shadow);
  padding: 2em 1em 1.5em 1em;
  margin-bottom: 1em;
}
.summary-section h3 {
  margin-bottom: 1.5em;
  color: var(--primary);
  font-size: 1.3em;
  font-weight: 700;
}
.summary-cards {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
  gap: 1.5em;
}
.summary-card {
  background: var(--bg);
  border-radius: 12px;
  box-shadow: 0 2px 8px rgba(60,60,60,0.06);
  padding: 1.2em 0.5em;
  display: flex;
  flex-direction: column;
  align-items: center;
  transition: box-shadow 0.2s;
}
.summary-card:hover {
  box-shadow: 0 6px 24px rgba(60,60,60,0.13);
}
.summary-label {
  color: #222;
  font-size: 1em;
  margin-bottom: 0.3em;
  font-weight: 700;
  letter-spacing: 0.2px;
}
.summary-value {
  font-size: 1.5em;
  font-weight: 900;
  color: var(--primary);
}
.usage-graph-section {
  margin: 2em auto 0 auto;
  max-width: 700px;
  padding: 2.5em 1.5em 2em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: rgba(255,255,255,0.85);
  box-shadow: 0 2px 12px rgba(60,60,60,0.08);
}
.usage-graph {
  width: 100%;
  height: 240px;
  margin-top: 1em;
  background: none;
  display: block;
  cursor: pointer;
}
.section-title {
  margin-bottom: 1.2em;
  font-size: 1.3em;
  font-weight: 700;
  color: var(--primary, #5B2A9D);
}
.message {
  color: var(--accent);
  margin-top: 1em;
  font-weight: 600;
}
.logout-btn {
  margin-left: 2em;
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
/* Ensure navbar is always visible */
:deep(.navbar) {
  background: #fff !important;
  box-shadow: 0 2px 12px rgba(60,60,60,0.10) !important;
}
:deep(.nav-links a), :deep(.nav-links .router-link-active) {
  color: var(--primary) !important;
}
:deep(.nav-links a:hover), :deep(.nav-links .router-link-active) {
  background: #f0f4ff !important;
  color: var(--primary-light) !important;
}
@media (max-width: 700px) {
  .dashboard-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  .summary-section {
    padding: 1.2em 0.2em 1em 0.2em;
  }
  .summary-cards {
    grid-template-columns: 1fr 1fr;
    gap: 0.7em;
  }
  .dashboard-title {
    font-size: 1.3em;
  }
  .logout-btn {
    padding: 0.7em 1.2em;
    font-size: 1em;
  }
}
@media (max-width: 480px) {
  .summary-cards {
    grid-template-columns: 1fr;
  }
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
</style> 