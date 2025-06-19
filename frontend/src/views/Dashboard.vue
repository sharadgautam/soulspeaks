<template>
  <div class="dashboard-container">
    <h2>Welcome to SoulSpeaks Cloud PBX!</h2>
    <form @submit.prevent="searchDids">
      <label>Country:</label>
      <input v-model="country" placeholder="US" />
      <button type="submit">Search DIDs</button>
    </form>
    <div v-if="dids.length">
      <h3>Available DIDs</h3>
      <ul>
        <li v-for="did in dids" :key="did.number">
          {{ did.number }} ({{ did.country }})
          <button @click="buyDid(did.number)">Buy</button>
        </li>
      </ul>
    </div>
    <p v-if="message" class="message">{{ message }}</p>
    <button @click="$emit('logout')">Logout</button>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'Dashboard',
  data() {
    return {
      country: 'US',
      dids: [],
      message: ''
    };
  },
  methods: {
    async searchDids() {
      this.message = '';
      try {
        const res = await axios.post('/api/did/search', { country: this.country });
        this.dids = res.data.dids || [];
      } catch (e) {
        this.message = 'Failed to search DIDs.';
      }
    },
    async buyDid(number) {
      this.message = '';
      try {
        const res = await axios.post('/api/did/buy', { number });
        if (res.data.success) {
          this.message = `Successfully bought DID: ${res.data.number}`;
        } else {
          this.message = 'Failed to buy DID.';
        }
      } catch (e) {
        this.message = 'Failed to buy DID.';
      }
    }
  }
};
</script>

<style scoped>
.dashboard-container {
  max-width: 600px;
  margin: 60px auto;
  padding: 2em;
  border: 1px solid #ccc;
  border-radius: 8px;
  text-align: center;
}
.message {
  color: green;
  margin-top: 1em;
}
</style> 