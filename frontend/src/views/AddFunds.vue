<template>
  <PageLayout>
    <div class="add-funds-container">
      <h2 class="gradient-text">Add Funds</h2>
      <form class="glass add-funds-form" @submit.prevent="addFunds">
        <input v-model.number="amount" type="number" min="1" class="fancy-input" placeholder="Amount (USD)" required />
        <button type="submit" class="fancy-btn">Add Funds</button>
      </form>
      <p v-if="message" class="message">{{ message }}</p>
    </div>
  </PageLayout>
</template>

<script>
import PageLayout from '../components/PageLayout.vue';
export default {
  name: 'AddFunds',
  components: { PageLayout },
  data() {
    return {
      amount: 0,
      message: ''
    };
  },
  methods: {
    addFunds() {
      if (this.amount > 0) {
        // In a real app, call backend API to add funds
        this.message = `Successfully added $${this.amount.toFixed(2)} to your account.`;
        this.amount = 0;
      } else {
        this.message = 'Please enter a valid amount.';
      }
    }
  }
};
</script>

<style scoped>
.add-funds-container {
  max-width: 400px;
  margin: 60px auto;
  padding: 2.5em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: none;
}
.add-funds-form {
  display: flex;
  flex-direction: column;
  gap: 1.2em;
  align-items: center;
  padding: 2em 1em 1.5em 1em;
  margin-bottom: 1.5em;
}
.fancy-input {
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
  margin: 0.5em auto;
  width: 100%;
  max-width: 300px;
  display: block;
  text-align: center;
}
.fancy-input:focus {
  border-color: var(--accent, #00BFAE);
  box-shadow: 0 4px 16px rgba(0,191,174,0.10);
}
.fancy-input:hover {
  border-color: var(--primary-light, #7C4DFF);
}
.fancy-input::placeholder {
  text-align: center;
}
.fancy-btn {
  min-width: 140px;
  font-size: 1.1em;
  font-weight: 700;
  border-radius: 8px;
  border: none;
  background: linear-gradient(90deg, #5B2A9D 0%, #00BFAE 100%);
  color: #fff;
  box-shadow: 0 4px 16px rgba(91,42,157,0.13);
  cursor: pointer;
  outline: none;
  padding: 0.8em 2em;
  position: relative;
  overflow: hidden;
  transition: background 0.2s, color 0.2s, box-shadow 0.2s, transform 0.13s;
}
.fancy-btn::after {
  content: '';
  position: absolute;
  left: -50%;
  top: -50%;
  width: 200%;
  height: 200%;
  background: linear-gradient(120deg, #00BFAE 0%, #7C4DFF 100%);
  opacity: 0;
  transition: opacity 0.3s;
  z-index: 0;
}
.fancy-btn:hover {
  background: linear-gradient(90deg, #00BFAE 0%, #7C4DFF 100%);
  color: #fff;
  box-shadow: 0 8px 32px rgba(91,42,157,0.18);
  transform: translateY(-2px) scale(1.04);
}
.fancy-btn:active {
  transform: scale(0.98);
}
.message {
  color: var(--accent, #00BFAE);
  margin-top: 1em;
  font-weight: 600;
}
@media (max-width: 700px) {
  .add-funds-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  .add-funds-form {
    padding: 1em 0.2em;
  }
}
</style> 