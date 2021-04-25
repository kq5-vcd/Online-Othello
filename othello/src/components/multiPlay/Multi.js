import React from 'react'
import Table from './Table'

const Multi = () => {

    const search = () => {
        console.log(document.getElementById('search').innerText)
    }
    
    return (
        <>
            <div className="toolbar">
                <input id="search" type="text" placeholder="Enter room ID" onChange={search}/>
                <button className="search">Search</button>
                <button className="create">Create new room</button>
            </div>

            <div className="table">
                <Table />
            </div>
        </>
    )
}

export default Multi