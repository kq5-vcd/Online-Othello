import React from 'react'

// class Square extends React.Component {
    
//     constructor(props) {
//         super(props)
//     }

//     render() {
//         return(

//         )
//     }
// }
const Square = ({ value, onClick }) => {
    const style = "squares"

    if (value != null) {
        return (
            <button className={style} onClick={onClick}>
                <div className={'a' + value}>
                </div>
            </button>
        )
    } else {
        return (
            <button className={style} onClick={onClick}>
            </button>
        )
    }
}

export default Square
