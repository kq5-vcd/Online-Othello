import React from 'react'

class Popup extends React.Component {
    constructor(props) {
        super(props)
    }

    render() {
        return (this.props.trigger) ? (
            <div className='popup'>
                <div className='popup-inner'>
                {this.props.children}
                </div>
            </div>
        ) : ''
    }
}

export default Popup