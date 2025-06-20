<template>
  <PageLayout>
    <div class="manage-dids-container">
      <h2 class="gradient-text">Manage DIDs</h2>
      <form v-if="ownedDids.length && trunks.length" class="glass" @submit.prevent="assignTrunk">
        <label>DID:</label>
        <select v-model="selectedDid">
          <option v-for="did in ownedDids" :key="did">{{ did }}</option>
        </select>
        <label>Trunk:</label>
        <select v-model="selectedTrunk">
          <option v-for="trunk in trunks" :key="trunk">{{ trunk }}</option>
        </select>
        <button type="submit" class="nav-btn">Assign</button>
      </form>
      <ul v-if="assignments.length" class="assignment-list">
        <li v-for="(a, idx) in assignments" :key="idx" class="card glass assignment-card">
          <span class="assignment-label">DID:</span> {{ a.did }}<br />
          <span class="assignment-label">Trunk:</span> {{ a.trunk }}
        </li>
      </ul>
      <p v-if="message" class="message">{{ message }}</p>
    </div>
  </PageLayout>
</template>

<script>
import PageLayout from '../components/PageLayout.vue';
export default {
  name: 'ManageDids',
  components: { PageLayout },
  data() {
    return {
      ownedDids: [],
      trunks: [],
      selectedDid: '',
      selectedTrunk: '',
      assignments: [],
      message: ''
    };
  },
  methods: {
    assignTrunk() {
      if (this.selectedDid && this.selectedTrunk) {
        this.assignments.push({ did: this.selectedDid, trunk: this.selectedTrunk });
        this.message = `Assigned trunk ${this.selectedTrunk} to DID ${this.selectedDid}`;
        this.selectedDid = '';
        this.selectedTrunk = '';
      } else {
        this.message = 'Please select both a DID and a trunk.';
      }
    }
  }
};
</script>

<style scoped>
.manage-dids-container {
  max-width: 600px;
  margin: 60px auto;
  padding: 2.5em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: none;
}
form.glass {
  margin-bottom: 2em;
  padding: 1.5em 1em;
  display: flex;
  flex-wrap: wrap;
  gap: 1em 2em;
  align-items: center;
  justify-content: center;
}
.assignment-list {
  list-style: none;
  padding: 0;
  margin: 0;
  display: flex;
  flex-direction: column;
  gap: 1.2em;
}
.assignment-card {
  padding: 1.2em 1em;
  border-radius: 14px;
  font-size: 1.1em;
  background: rgba(255,255,255,0.85);
  border: 1.5px solid var(--primary, #5B2A9D);
  box-shadow: 0 2px 12px rgba(60,60,60,0.08);
  transition: box-shadow 0.2s, border 0.2s, transform 0.13s;
}
.assignment-card:hover {
  box-shadow: 0 8px 32px rgba(60,60,60,0.18);
  border-color: var(--accent, #00BFAE);
  transform: translateY(-4px) scale(1.03);
}
.assignment-label {
  color: var(--primary, #5B2A9D);
  font-weight: 700;
}
.message {
  color: var(--accent, #00BFAE);
  margin-top: 1em;
  font-weight: 600;
}
@media (max-width: 700px) {
  .manage-dids-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  form.glass {
    flex-direction: column;
    gap: 0.7em;
    padding: 1em 0.2em;
  }
}
</style> 