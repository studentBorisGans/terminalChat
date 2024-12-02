import axios from 'axios';

const api = axios.create({
  baseURL: 'http://172.16.2.102:8001', // backend server URL
});

export default api;
