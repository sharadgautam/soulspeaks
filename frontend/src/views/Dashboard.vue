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

    <!-- Add Trunk Form -->
    <div class="trunks-section">
      <h3>Add Destination Trunk (SIP URI)</h3>
      <form @submit.prevent="addTrunk">
        <input v-model="newTrunk" placeholder="e.g. sip:trunk@sip.example.com" required />
        <button type="submit">Add Trunk</button>
      </form>
      <ul v-if="trunks.length">
        <li v-for="(trunk, idx) in trunks" :key="idx">{{ trunk }}</li>
      </ul>
    </div>

    <!-- Assign Trunk to DID -->
    <div v-if="ownedDids.length && trunks.length" class="assign-section">
      <h3>Assign Trunk to DID</h3>
      <form @submit.prevent="assignTrunk">
        <label>DID:</label>
        <select v-model="selectedDid">
          <option v-for="did in ownedDids" :key="did">{{ did }}</option>
        </select>
        <label>Trunk:</label>
        <select v-model="selectedTrunk">
          <option v-for="trunk in trunks" :key="trunk">{{ trunk }}</option>
        </select>
        <button type="submit">Assign</button>
      </form>
      <ul v-if="assignments.length">
        <li v-for="(a, idx) in assignments" :key="idx">
          DID: {{ a.did }} → Trunk: {{ a.trunk }}
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
      message: '',
      newTrunk: '',
      trunks: [],
      ownedDids: [],
      selectedDid: '',
      selectedTrunk: '',
      assignments: []
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
          this.ownedDids.push(res.data.number);
        } else {
          this.message = 'Failed to buy DID.';
        }
      } catch (e) {
        this.message = 'Failed to buy DID.';
      }
    },
    addTrunk() {
      if (this.newTrunk && !this.trunks.includes(this.newTrunk)) {
        this.trunks.push(this.newTrunk);
        this.newTrunk = '';
      }
    },
    assignTrunk() {
      if (this.selectedDid && this.selectedTrunk) {
        this.assignments.push({ did: this.selectedDid, trunk: this.selectedTrunk });
        this.selectedDid = '';
        this.selectedTrunk = '';
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