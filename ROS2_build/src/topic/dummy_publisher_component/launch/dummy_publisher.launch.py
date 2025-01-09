from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    launch_description = LaunchDescription()
    
    dummy_publisher_node = ComposableNodeContainer(
        name='dummy_publisher_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='dummy_publisher_component',
                plugin='dummy_publisher::DummyPublisherComponent',
                name='dummy_publisher_component'
            ),
        ],
        output='screen'
    )
    launch_description.add_action(dummy_publisher_node)

    return launch_description
