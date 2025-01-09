from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    launch_description = LaunchDescription()
    
    dummy_subscriber_node = ComposableNodeContainer(
        name='dummy_subscriber_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='dummy_subscriber_component',
                plugin='dummy_subscriber_component::DummySubscriberComponent',
                name='dummy_subscriber_component'
            ),
        ],
        output='screen'
    )
    launch_description.add_action(dummy_subscriber_node)

    return launch_description