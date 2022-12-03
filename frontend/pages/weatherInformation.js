import React, { useState, useEffect } from 'react';



export default function weatherInformation() 
{
    const [posts, setPosts] = useState([]);

    const fetchPost = () => {
        console.log("doing fetch")
    const response = fetch(
        "http://localhost:3000/weather/all"
      )
      .then(res => res.json())
      .then(data=> setPosts(data.weatherData));
    }

    useEffect(() => {
        fetchPost();
    }, []);


    const fetchTemp = () =>{
        let temp = []
        for(let i =0; i < posts.length; i++){
            temp.push(posts[i].data.state.reported.temp)
        }
        return temp;
      } 
      
      const fetchTime = () =>{
        let time = []
        for(let i =0; i < posts.length; i++){
            time.push(posts[i].timestamp)
        }
        return time;
      } 
    
      const labels = fetchTime();


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
      

      
    const data = {
        labels,
        datasets: [
          {
            label: 'Dataset 1',
            data: fetchTemp(),
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
          }
        ],
      };


    return(   
        <div className={styles.container}> 
        <main className={styles.main}>
            <Line options={options} data={data} width={100} height={50}></Line>
        </main>
      </div>
    )
}
 