<template>
  <PageLayout>
    <div class="main-content transactions-container">
      <h2 class="gradient-text">Transactions</h2>
      <div class="glass table-wrapper">
        <table class="transactions-table">
          <thead>
            <tr>
              <th>Date</th>
              <th>Type</th>
              <th>Amount</th>
              <th>Description</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(txn, idx) in transactions" :key="idx">
              <td>{{ txn.date }}</td>
              <td>{{ txn.type }}</td>
              <td>${{ txn.amount.toFixed(2) }}</td>
              <td>{{ txn.description }}</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </PageLayout>
</template>

<script>
import PageLayout from '../components/PageLayout.vue';

export default {
  name: 'Transactions',
  components: { PageLayout },
  data() {
    return {
      transactions: [
        { date: '2024-06-01', type: 'Credit', amount: 50.00, description: 'Added funds' },
        { date: '2024-06-02', type: 'Debit', amount: 1.00, description: 'Bought DID: (573) 787-5703' },
        { date: '2024-06-03', type: 'Debit', amount: 1.00, description: 'Bought DID: (314) 555-1234' },
        { date: '2024-06-04', type: 'Credit', amount: 25.00, description: 'Added funds' }
      ]
    };
  }
};
</script>

<style scoped>
.transactions-container {
  max-width: 900px;
  margin: 60px auto;
  padding: 2.5em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: none;
}
.table-wrapper {
  margin: 2em auto 0 auto;
  border-radius: 18px;
  box-shadow: 0 4px 32px rgba(60,60,60,0.10);
  overflow-x: auto;
  background: rgba(255,255,255,0.85);
  padding: 2em 1em 1.5em 1em;
}
.transactions-table {
  width: 100%;
  border-collapse: separate;
  border-spacing: 0;
  margin: 0 auto;
  font-size: 1.08em;
  background: transparent;
  border-radius: 14px;
  overflow: hidden;
  box-shadow: 0 2px 12px rgba(60,60,60,0.08);
  transition: box-shadow 0.2s;
}
.transactions-table th, .transactions-table td {
  padding: 1em 1.5em;
  border-bottom: 1.5px solid #ece6f6;
  transition: background 0.18s, color 0.18s;
}
.transactions-table th {
  background: linear-gradient(90deg, #f0f4ff 0%, #e0f7fa 100%);
  color: var(--primary, #5B2A9D);
  font-weight: 800;
  font-size: 1.1em;
  border-top: none;
}
.transactions-table tr:last-child td {
  border-bottom: none;
}
.transactions-table tbody tr {
  background: transparent;
  transition: background 0.18s, color 0.18s, box-shadow 0.18s;
}
.transactions-table tbody tr:hover {
  background: linear-gradient(90deg, #e0f7fa 0%, #f0f4ff 100%);
  color: var(--primary, #5B2A9D);
  box-shadow: 0 4px 24px rgba(91,42,157,0.10);
  transform: scale(1.01);
  z-index: 2;
  position: relative;
}
.transactions-table td {
  color: #333;
  font-weight: 600;
}
@media (max-width: 700px) {
  .transactions-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  .table-wrapper {
    padding: 1em 0.2em;
  }
  .transactions-table th, .transactions-table td {
    padding: 0.7em 0.5em;
    font-size: 0.98em;
  }
}
</style> 