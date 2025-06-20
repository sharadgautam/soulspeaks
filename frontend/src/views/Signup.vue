<template>
  <div class="signup-container">
    <h2>Sign Up for SoulSpeaks Cloud PBX</h2>
    <form @submit.prevent="onSignup">
      <div>
        <label>Company Name</label>
        <input v-model="form.company" required />
      </div>
      <div>
        <label>Customer Name</label>
        <input v-model="form.name" required />
      </div>
      <div>
        <label>Email</label>
        <input v-model="form.email" type="email" required />
      </div>
      <div>
        <label>Username</label>
        <input v-model="form.username" required />
      </div>
      <div>
        <label>Phone</label>
        <input v-model="form.phone" required />
      </div>
      <div>
        <label>Country</label>
        <select v-model="form.country" @change="onCountryChange" required>
          <option value="">Select Country</option>
          <option v-for="c in countries" :key="c.code" :value="c.code">{{ c.name }}</option>
        </select>
      </div>
      <div>
        <label>State</label>
        <select v-model="form.state" required>
          <option value="">Select State</option>
          <option v-for="s in states" :key="s" :value="s">{{ s }}</option>
        </select>
      </div>
      <div>
        <label>Full Address</label>
        <input v-model="form.address" required />
      </div>
      <div>
        <label>Zip</label>
        <input v-model="form.zip" required />
      </div>
      <div>
        <label>Password</label>
        <input v-model="form.password" type="password" required />
      </div>
      <button type="submit">Sign Up</button>
      <p v-if="message" :class="{ error: error }">{{ message }}</p>
    </form>
  </div>
</template>

<script>
import axios from 'axios';

const countryStateData = {
  US: ["Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming"],
  IN: ["Andhra Pradesh", "Arunachal Pradesh", "Assam", "Bihar", "Chhattisgarh", "Goa", "Gujarat", "Haryana", "Himachal Pradesh", "Jharkhand", "Karnataka", "Kerala", "Madhya Pradesh", "Maharashtra", "Manipur", "Meghalaya", "Mizoram", "Nagaland", "Odisha", "Punjab", "Rajasthan", "Sikkim", "Tamil Nadu", "Telangana", "Tripura", "Uttar Pradesh", "Uttarakhand", "West Bengal"],
  // Add more countries/states as needed
};

export default {
  name: 'Signup',
  data() {
    return {
      form: {
        company: '', name: '', email: '', phone: '', country: '', state: '', address: '', zip: '', password: '', username: ''
      },
      countries: [
        { code: 'US', name: 'United States' },
        { code: 'IN', name: 'India' }
        // Add more countries as needed
      ],
      states: [],
      message: '',
      error: false
    };
  },
  methods: {
    onCountryChange() {
      this.states = countryStateData[this.form.country] || [];
      this.form.state = '';
    },
    async onSignup() {
      this.message = '';
      this.error = false;
      try {
        const res = await axios.post('/api/signup', this.form);
        if (res.data.success) {
          this.message = 'Signup successful! Please check your email to verify your account.';
        } else {
          this.message = res.data.error || 'Signup failed.';
          this.error = true;
        }
      } catch (e) {
        this.message = 'Signup failed.';
        this.error = true;
      }
    }
  }
};
</script>

<style scoped>
.signup-container {
  max-width: 400px;
  margin: 60px auto;
  padding: 2em;
  border: 1px solid #ccc;
  border-radius: 8px;
  background: #fff;
  box-shadow: 0 2px 16px rgba(0,0,0,0.07);
}
form > div {
  margin-bottom: 1em;
  text-align: left;
}
label {
  display: block;
  margin-bottom: 0.3em;
  font-weight: bold;
}
input, select {
  width: 100%;
  padding: 0.5em;
  border-radius: 4px;
  border: 1px solid #bbb;
}
button {
  width: 100%;
  padding: 0.8em;
  background: #42b983;
  color: #fff;
  border: none;
  border-radius: 4px;
  font-size: 1.1em;
  font-weight: bold;
  cursor: pointer;
  transition: background 0.2s;
}
button:hover {
  background: #369870;
}
.error {
  color: red;
  margin-top: 1em;
}
</style> 