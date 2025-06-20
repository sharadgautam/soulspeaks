import { createRouter, createWebHistory } from 'vue-router';
import Home from './views/Home.vue';
import Login from './views/Login.vue';
import Dashboard from './views/Dashboard.vue';
import Signup from './views/Signup.vue';
import BuyDids from './views/BuyDids.vue';
import AddTrunk from './views/AddTrunk.vue';
import ManageDids from './views/ManageDids.vue';
import AddFunds from './views/AddFunds.vue';
import Transactions from './views/Transactions.vue';
import SipClient from './views/SipClient.vue';

const routes = [
  { path: '/', component: Home },
  { path: '/login', component: Login },
  { path: '/dashboard', component: Dashboard },
  { path: '/signup', component: Signup },
  { path: '/buy-dids', component: BuyDids },
  { path: '/add-trunk', component: AddTrunk },
  { path: '/manage-dids', component: ManageDids },
  { path: '/add-funds', component: AddFunds },
  { path: '/transactions', component: Transactions },
  { path: '/sip-client', component: SipClient }
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

export default router; 