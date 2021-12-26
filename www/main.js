console.log('hi');

const elements = {};

elements.rfidId = document.querySelector('.rfid-id');
elements.rfidRefresh = document.querySelector('.rfid-refresh');

elements.rfidRefresh.addEventListener('click', () => {
	console.log(window.location.origin);
});