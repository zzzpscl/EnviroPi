import { Chart, registerables } from 'https://cdn.jsdelivr.net/npm/chart.js@4/+esm';
Chart.register(...registerables);
// ^ Made with Claude ^

const ctx = document.getElementById('myChart');

new Chart(ctx, {
    type: 'line',
    data: {
        labels: ['11:30', '12:30', '13:30', '14:30', '15:30', '16:20'], //Uhrzeit
        datasets: [{
            label: 'Temperatur',
            data: [12,3, 13, 5, 12, 17, 22], //Temperatur
            borderWidth: 2,
            tension: 0.4
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});