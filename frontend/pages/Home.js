import * as React from 'react';
import { styled } from '@mui/material/styles';
import Box from '@mui/material/Box';
import Paper from '@mui/material/Paper';
import Grid from '@mui/material/Grid';
import Container from '@mui/material/Container';
import WeatherDisplay from '../helpers/weather_component';

import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
  } from 'chart.js';
  import { Line } from 'react-chartjs-2';
  
  import styles from '../styles/Home.module.css';
  
  ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend
  );
  


const Item = styled(Paper)(({ theme }) => ({
  backgroundColor: theme.palette.mode === 'dark' ? '#1A2027' : '#fff',
  ...theme.typography.body2,
  padding: theme.spacing(1),
  textAlign: 'center',
  color: theme.palette.text.secondary,
}));




export default function Home() 
{
    const locale = 'en';
    const [today, setDate] = React.useState(new Date()); // Save the current date to be able to trigger an update
    const [weather, setWeather] = React.useState([]);
    const [temp, setTemp] = React.useState([]);
    const [Humidity, setHumidity] = React.useState([]);
    const [label, setLabel] = React.useState([]);
    const [weatherCondition, setWeatherCondition] = React.useState("");

    const fetchPost = () => {
        console.log("doing fetch")
    const response = fetch(
        "http://localhost:3000/weather/all"
      )
      .then(res => res.json())
      .then(data=> setWeather(data.weatherData));
      console.log("fetch weather");
    }

    const fetchTemp = () =>{
        let temp = []
        for(let i =0; i < weather.length; i++){
            temp.push(parseFloat(weather[i].data.state.reported.temperature))
        }
        setTemp(temp);
    } 

    const fetchHumidity = () =>{
        let temp = []
        for(let i =0; i < weather.length; i++){
            temp.push(parseFloat(weather[i].data.state.reported.humidity))
        }
        setHumidity(temp);
    } 
          
    const fetchTime = () =>{
        let time = []
        for(let i =0; i < weather.length; i++){
            time.push(weather[i].data.time)
        }
        console.log(time);
        return setLabel(time);
    } 

    const fetchRainData = () =>{
      let rain = "";  
      rain = weather[weather.length -1].data.rain;

      if(rain == ""){
        setWeatherCondition("sunny");
      }
      if (rain == "yes"){
        setWeatherCondition("rainy"); 
      }
      else if(temp[temp.length- 1] > 17 && temp[temp.length- 1] < 24){
        setWeatherCondition("sunny");
      }
      else if(temp[temp.length- 1] < 2){
        setWeatherCondition("snowy");
      }
    }

    React.useEffect(()=>{
        const timer = setInterval(() => {          
            fetchPost();
            fetchTime();
            fetchTemp();
            fetchHumidity();
            fetchRainData();
          }, 12 * 1000);
          return () => {
            clearInterval(timer);
        }
    }, [weather]);

    React.useEffect(()=>{
      const timer = setInterval(() => {          
        fetchRainData();
      }, 12 * 1000);
      return () => {
        clearInterval(timer);
    }
},[weather]);

    React.useEffect(() => {
        const timer = setInterval(() => { // Creates an interval which will update the current data every minute
        // This will trigger a rerender every component that uses the useDate hook.
        setDate(new Date());
        
      }, 60 * 1000);
      return () => {
        clearInterval(timer); // Return a funtion to clear the timer so that it will stop being called on unmount
      }
    }, [today]);

    const day = today.toLocaleDateString(locale, { weekday: 'long' });
    const date = `${day}, ${today.getDate()} ${today.toLocaleDateString(locale, { month: 'long' })}\n\n`;
  
    const hour = today.getHours();
    const wish = `Good ${(hour < 12 && 'Morning') || (hour < 17 && 'Afternoon') || 'Evening'}, `;
  
    const time = today.toLocaleTimeString(locale, { hour: 'numeric', hour12: true, minute: 'numeric' });



      const labels = label;


    const options = {
        responsive: true,
        plugins: {
          legend: {
            position: 'top',
          },
          title: {
            display: true,
            text: 'Weather information',
          },
        },
      };
      

      
    const tempData = {
        labels,
        datasets: [
          {
            label: 'Temperature',
            data: temp,
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
          }
        ],
      };

      const humidityData = {
        labels,
        datasets: [
          {
            label: 'Humidity',
            data: Humidity,
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
          }
        ],
      };


    return(   
        <>
    <Container>
        <h1>Weather information</h1>
        <Box sx={{ flexGrow: 2}}>
      <Grid container spacing={3} columns={6}>
        <Grid item xs={1.5}>
            <Item>
                {time}
            </Item>
        </Grid>
        <Grid item xs={1.5}>
            <Item>
                {date}
            </Item>
        </Grid>
        <Grid item xs={1}>
          <WeatherDisplay width={200} height={50} currentTemperature={parseFloat(temp[temp.length- 1]).toFixed()} currentCondition={weatherCondition} />
        </Grid>
        <Grid item xs={1}>
            <Item>Minimum: {Math.min(...temp).toFixed()}°</Item>
        </Grid>
        <Grid item xs={1}>
            <Item>Maximum: {Math.max(...temp).toFixed()}°</Item>
        </Grid>
        <Grid item xs={6}>
            <Item><Line options={options} data={tempData}></Line></Item>
        </Grid>
        <Grid item xs={6}>
            <Item><Line options={options} data={humidityData}></Line></Item>
        </Grid>
      </Grid>
    </Box>
    </Container>
        </>
    )
}
 